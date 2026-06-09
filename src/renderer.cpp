#include "renderer.h"
#include <vector>
#include <algorithm>

OverlayRenderer g_Renderer;

OverlayRenderer::OverlayRenderer()
    : device_(nullptr), texture_(nullptr), line_vbo_(nullptr),
      tex_width_(0), tex_height_(0), font_height_(16),
      mem_dc_(nullptr), mem_bmp_(nullptr), font_(nullptr), bits_(nullptr) {
}

OverlayRenderer::~OverlayRenderer() {
    Release();
}

bool OverlayRenderer::Init(IDirect3DDevice9* device) {
    if (!device) return false;
    device_ = device;
    device_->AddRef();

    // 创建 GDI 字体
    font_ = CreateFontA(font_height_, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
    if (!font_) return false;

    // 创建内存 DC
    mem_dc_ = CreateCompatibleDC(nullptr);
    if (!mem_dc_) return false;
    SelectObject(mem_dc_, font_);
    SetBkMode(mem_dc_, TRANSPARENT);
    SetTextColor(mem_dc_, RGB(255, 255, 255));

    // 创建线条 VBO（6 个顶点用于 3 条线）
    if (FAILED(device_->CreateVertexBuffer(sizeof(ScreenVertex) * 6, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        D3DFVF_SCREENVERTEX, D3DPOOL_DEFAULT, &line_vbo_, nullptr))) {
        line_vbo_ = nullptr;
    }

    return true;
}

void OverlayRenderer::Release() {
    if (texture_) { texture_->Release(); texture_ = nullptr; }
    if (line_vbo_) { line_vbo_->Release(); line_vbo_ = nullptr; }
    if (mem_bmp_) { DeleteObject(mem_bmp_); mem_bmp_ = nullptr; }
    if (mem_dc_) { DeleteDC(mem_dc_); mem_dc_ = nullptr; }
    if (font_) { DeleteObject(font_); font_ = nullptr; }
    if (bits_) { bits_ = nullptr; } // bits owned by bitmap
    if (device_) { device_->Release(); device_ = nullptr; }
}

bool OverlayRenderer::EnsureTexture(int req_w, int req_h) {
    if (!device_) return false;

    // 对齐到 4 的倍数
    req_w = (req_w + 3) & ~3;
    req_h = (req_h + 3) & ~3;

    // 如果已有纹理且足够大，复用
    if (texture_ && tex_width_ >= req_w && tex_height_ >= req_h)
        return true;

    // 释放旧纹理和 bitmap
    if (texture_) { texture_->Release(); texture_ = nullptr; }
    if (mem_bmp_) { DeleteObject(mem_bmp_); mem_bmp_ = nullptr; bits_ = nullptr; }

    // 扩大到 2 倍预留空间
    tex_width_ = (std::max)(req_w * 2, 256);
    tex_height_ = (std::max)(req_h * 2, 64);

    // 创建 D3D 纹理
    if (FAILED(device_->CreateTexture(tex_width_, tex_height_, 1, D3DUSAGE_DYNAMIC,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture_, nullptr)))
        return false;

    // 创建匹配的 GDI bitmap（32-bit 每像素）
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = tex_width_;
    bmi.bmiHeader.biHeight = -tex_height_;  // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    mem_bmp_ = CreateDIBSection(mem_dc_, &bmi, DIB_RGB_COLORS, &bits_, nullptr, 0);
    if (!mem_bmp_) return false;

    SelectObject(mem_dc_, mem_bmp_);
    SelectObject(mem_dc_, font_);
    SetBkMode(mem_dc_, TRANSPARENT);
    SetTextColor(mem_dc_, RGB(255, 255, 255));
    return true;
}

void OverlayRenderer::RenderTextToTexture(const char* text, int& out_w, int& out_h) {
    if (!device_ || !mem_dc_ || !text || !*text) { out_w = 0; out_h = 0; return; }

    // 计算文字尺寸
    SIZE sz = {};
    std::string textStr(text);
    // 处理多行
    int maxW = 0, totalH = 0;
    const char* p = text;
    while (*p) {
        const char* next = strchr(p, '\n');
        int len = next ? (int)(next - p) : (int)strlen(p);

        char line[1024];
        memcpy(line, p, len); line[len] = 0;

        SIZE lineSz = {};
        GetTextExtentPoint32A(mem_dc_, line, len, &lineSz);
        if (lineSz.cx > maxW) maxW = lineSz.cx;
        totalH += font_height_ + 2;

        p = next ? next + 1 : p + len;
    }
    if (maxW == 0) maxW = 100;
    if (totalH == 0) totalH = font_height_ + 2;

    out_w = maxW + 4;
    out_h = totalH + 4;

    if (!EnsureTexture(out_w + 4, out_h + 4))
        return;

    if (out_w > tex_width_ || out_h > tex_height_)
        return;

    // 清空背景（全透明）
    memset(bits_, 0, tex_width_ * tex_height_ * 4);

    // 绘制文字到 bitmap
    int curY = 2;
    p = text;
    while (*p) {
        const char* next = strchr(p, '\n');
        int len = next ? (int)(next - p) : (int)strlen(p);
        if (len > 0) {
            char line[1024];
            memcpy(line, p, len); line[len] = 0;
            TextOutA(mem_dc_, 2, curY, line, len);
        }
        curY += font_height_ + 2;
        p = next ? next + 1 : p + len;
    }

    // 将 GDI bitmap 数据复制到 D3D 纹理
    D3DLOCKED_RECT locked = {};
    if (FAILED(texture_->LockRect(0, &locked, nullptr, D3DLOCK_DISCARD)))
        return;

    // 转换 ARGB: GDI 是 0x00BBGGRR, D3D 需要 0xAARRGGBB
    // 将 GDI 像素中非黑色 (0x00000000) 的部分转换为带 Alpha 的颜色
    BYTE* src = (BYTE*)bits_;
    BYTE* dst = (BYTE*)locked.pBits;
    for (int y = 0; y < out_h; y++) {
        for (int x = 0; x < out_w; x++) {
            int si = (y * tex_width_ + x) * 4;
            int di = y * locked.Pitch + x * 4;
            BYTE b = src[si + 0];
            BYTE g = src[si + 1];
            BYTE r = src[si + 2];
            BYTE a = src[si + 3]; // not used by GDI

            // 如果像素非全黑，设为白色带 Alpha（颜色由顶点着色指定）
            if (r || g || b) {
                dst[di + 0] = b;     // B
                dst[di + 1] = g;     // G
                dst[di + 2] = r;     // R
                dst[di + 3] = 255;   // A
            } else {
                dst[di + 0] = 0;
                dst[di + 1] = 0;
                dst[di + 2] = 0;
                dst[di + 3] = 0;
            }
        }
    }

    texture_->UnlockRect(0);
}

void OverlayRenderer::DrawText(int x, int y, D3DCOLOR color, const char* fmt, ...) {
    if (!device_ || !mem_dc_) return;

    char buf[1024] = {};
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    int tw = 0, th = 0;
    RenderTextToTexture(buf, tw, th);
    if (!texture_ || tw == 0 || th == 0) return;

    // 绘制纹理四边形
    float fx = (float)x, fy = (float)y;
    float fw = (float)tw, fh = (float)th;

    ScreenVertex verts[4] = {
        { fx,     fy,     0.5f, 1.0f, color, 0, 0 },
        { fx+fw,  fy,     0.5f, 1.0f, color, 1, 0 },
        { fx,     fy+fh,  0.5f, 1.0f, color, 0, 1 },
        { fx+fw,  fy+fh,  0.5f, 1.0f, color, 1, 1 },
    };

    device_->SetTexture(0, texture_);
    device_->SetFVF(D3DFVF_SCREENVERTEX);
    device_->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    device_->SetRenderState(D3DRS_ZENABLE, FALSE);
    device_->SetRenderState(D3DRS_LIGHTING, FALSE);

    device_->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, verts, sizeof(ScreenVertex));

    device_->SetTexture(0, nullptr);
}

void OverlayRenderer::DrawLine(float x1, float y1, float x2, float y2, D3DCOLOR color) {
    if (!device_ || !line_vbo_) return;

    ScreenVertex verts[2] = {
        { x1, y1, 0.5f, 1.0f, color, 0, 0 },
        { x2, y2, 0.5f, 1.0f, color, 0, 0 },
    };

    device_->SetTexture(0, nullptr);
    device_->SetFVF(D3DFVF_SCREENVERTEX);
    device_->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    device_->SetRenderState(D3DRS_ZENABLE, FALSE);
    device_->SetRenderState(D3DRS_LIGHTING, FALSE);

    device_->DrawPrimitiveUP(D3DPT_LINELIST, 1, verts, sizeof(ScreenVertex));
}

void OverlayRenderer::DrawBox(float x, float y, float w, float h, D3DCOLOR color) {
    DrawLine(x, y, x + w, y, color);
    DrawLine(x + w, y, x + w, y + h, color);
    DrawLine(x + w, y + h, x, y + h, color);
    DrawLine(x, y + h, x, y, color);
}

void OverlayRenderer::DrawBorderBox(float x, float y, float w, float h, float thickness, D3DCOLOR color) {
    DrawBox(x, y, w, h, color);
    if (thickness > 1.0f) {
        DrawBox(x - 1, y - 1, w + 2, h + 2, color);
    }
}

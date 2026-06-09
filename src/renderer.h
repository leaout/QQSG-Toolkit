#pragma once
// ============================================================
// D3D9 文字/线条绘制（纯 D3D9，不依赖 d3dx9）
// 文字：GDI 渲染到内存 -> D3D 纹理 -> 屏幕
// 线条：D3D9 顶点缓冲
// ============================================================

#include <d3d9.h>
#include <string>
#include <cstdarg>
#include <cstdio>
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX

// 屏幕顶点结构
struct ScreenVertex {
    float x, y, z, rhw;
    D3DCOLOR color;
    float u, v;
};
#define D3DFVF_SCREENVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 简易 D3D9 Overlay 绘制器
class OverlayRenderer {
public:
    OverlayRenderer();
    ~OverlayRenderer();

    // 初始化（在 D3D9 设备就绪后调用）
    bool Init(IDirect3DDevice9* device);

    // 释放资源
    void Release();

    // 绘制文字（每帧调用）
    void DrawText(int x, int y, D3DCOLOR color, const char* fmt, ...);

    // 绘制线条
    void DrawLine(float x1, float y1, float x2, float y2, D3DCOLOR color);
    void DrawBox(float x, float y, float w, float h, D3DCOLOR color);
    void DrawBorderBox(float x, float y, float w, float h, float thickness, D3DCOLOR color);

private:
    IDirect3DDevice9* device_;
    IDirect3DTexture9*     texture_;
    IDirect3DVertexBuffer9* line_vbo_;

    int tex_width_;
    int tex_height_;
    int font_height_;

    HDC     mem_dc_;
    HBITMAP mem_bmp_;
    HFONT   font_;
    void*   bits_;

    bool EnsureTexture(int req_w, int req_h);
    void RenderTextToTexture(const char* text, int& out_w, int& out_h);
};

// 全局单例
extern OverlayRenderer g_Renderer;

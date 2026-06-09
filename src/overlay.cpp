#include "overlay.h"
#include "game_data.h"
#include "d3d9_manager.h"
#include "renderer.h"

void DrawOverlay() {
    // 使用 g_Renderer 进行绘制
    auto& r = g_Renderer;

    // ----- 示例：读取并显示装备耐久 -----
    int x = 10;
    int y = 50;

    r.DrawText(x, y, D3DCOLOR_ARGB(255, 255, 255, 0), "HAT耐久: %i",  Equipment::GetHeadCur()); y += 18;
    r.DrawText(x, y, D3DCOLOR_ARGB(255, 255, 255, 0), "NECK耐久: %i", Equipment::GetNeckCur()); y += 18;
    r.DrawText(x, y, D3DCOLOR_ARGB(255, 255, 255, 0), "FACE耐久: %i", Equipment::GetGlassCur()); y += 18;
    r.DrawText(x, y, D3DCOLOR_ARGB(255, 255, 255, 0), "WING耐久: %i", Equipment::GetWingCur()); y += 18;
    r.DrawText(x, y, D3DCOLOR_ARGB(255, 255, 255, 0), "UPPER耐久: %i", Equipment::GetUpperCur()); y += 18;
    r.DrawText(x, y, D3DCOLOR_ARGB(255, 255, 255, 0), "LOWER耐久: %i", Equipment::GetLowerCur()); y += 18;
    r.DrawText(x, y, D3DCOLOR_ARGB(255, 255, 255, 0), "WEAPON耐久: %i", Equipment::GetWeaponCur()); y += 18;

    // ----- 坐标与地图 -----
    DWORD mapId = Overworld::GetMapIndex();
    DWORD posX = Overworld::GetPosX();
    DWORD posY = Overworld::GetPosY();
    DWORD speed = Overworld::GetSpeedHor();
    DWORD convState = Overworld::GetConversationState();
    BYTE typing = Social::GetTypingState();

    r.DrawText(10, 5, D3DCOLOR_ARGB(255, 0, 255, 0), "QQSG-Toolkit v1.0");
    r.DrawText(10, 230, D3DCOLOR_ARGB(255, 0, 255, 255), "坐标: (%d, %d)", posX, posY);
    r.DrawText(10, 248, D3DCOLOR_ARGB(255, 0, 255, 255), "速度: %d", speed);
    r.DrawText(10, 266, D3DCOLOR_ARGB(255, 0, 255, 255), "地图ID: %d", mapId);
    r.DrawText(10, 284, D3DCOLOR_ARGB(255, 0, 255, 255), "对话状态: %d", convState);
    r.DrawText(10, 302, D3DCOLOR_ARGB(255, 0, 255, 255), "输入状态: %02x", typing);

    // 显示地图名称
    const char* mapName = "未知";
    switch (mapId) {
    case HOLE1: mapName = "洞窟 1"; break;
    case HOLE2: mapName = "洞窟 2"; break;
    case HOLE3: mapName = "洞窟 3"; break;
    case HOLE4: mapName = "洞窟 4"; break;
    case HOLE5: mapName = "洞窟 5"; break;
    case HOLE6: mapName = "洞窟 6"; break;
    case HOLEEND: mapName = "洞窟终点"; break;
    case ENTRYYANNIAODU: mapName = "雁鸟渡入口"; break;
    default: mapName = "野外"; break;
    }
    r.DrawText(g_D3D9.Width / 2, 10, D3DCOLOR_ARGB(255, 255, 0, 0), "当前位置: %s", mapName);

    r.DrawText(10, 330, D3DCOLOR_ARGB(255, 0, 255, 0), "Delete: 卸载");
}

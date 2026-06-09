#include "pathfinding.h"
#include "../d3d9_manager.h"
#include "../pattern.h"
#include "../game_data.h"
#include <cstring>
#include <vector>

PathFindTo_t g_PathFindTo = nullptr;

// =============================================================
// 逆向指南：如何用 Cheat Engine / IDA 找到寻路函数
// =============================================================
//
// 方法 A - Cheat Engine 追踪法（推荐，最易上手）
// =============================================================
// 1. 打开游戏 + CE
// 2. 在游戏中按 G 触发自动寻路（或者点小地图）
// 3. CE 搜索 "未知初始值" → 首次扫描
// 4. 每次 G 键寻路时扫描 "变动的数值" → 找到坐标相关地址
// 5. 找到目标X坐标的地址后，右键 → "找出是什么改写了这个地址"
// 6. 再次触发寻路，CE 会记录写这个地址的指令
// 7. 记下指令地址（例如 game.exe+123456），这就是寻路函数附近的代码
// 8. 用 IDA 或 CE "查看内存区域" 分析该函数的参数
//
// 方法 B - 特征码交叉引用法
// =============================================================
// 1. 用 CE 搜索 MoveLapMeta.Type 为 2, 4, 8 的那块道路数据区
// 2. 找到引用这些数据的函数
// 3. 往上回溯找到寻路入口函数
//
// 方法 C - 虚函数表索引法
// =============================================================
// 1. 游戏主对象 GameWorld 可能有一个 VTable
// 2. 遍历虚表函数，找到处理"移动"的那个
// 3. 调试验证参数
//
// 找到函数后：
// =============================================================
// 1. 在 call 附近复制 20-40 字节作为特征码
// 2. 填入下方 PATTERN_BYTES 和 PATTERN_MASK
// 3. 定义函数参数格式填入 PathFindTo_t
// =============================================================

// ----- 特征码配置（需要你逆向后填写） -----
// 用 ? 表示通配符（不确定的字节）
// 示例: "\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1"
static const char* PATTERN_BYTES = nullptr;  // 例如: "\x55\x8B\xEC\x83\xEC\x14\x53\x56\x57"
static const char* PATTERN_MASK = "xxxxxxxxx"; // 'x' = 需匹配, '?' = 通配

// 模块名（寻路函数所在模块，如 "game.exe", "QQSG.exe", "SGBase.dll"）
static const char* TARGET_MODULE = "game.exe";

// ----- 写法二：手动指定地址（如果已知固定偏移）-----
// 如果游戏版本固定，可以用模块基址 + 偏移
// 设置 g_PathFindTo = (PathFindTo_t)(moduleBase + 0x123456);
#define PATHFIND_FUNCTION_OFFSET 0x0  // ← 填入已知偏移

bool FindPathFindFunction() {
    // 尝试方式一：特征码扫描
    if (PATTERN_BYTES && PATTERN_MASK) {
        uintptr_t addr = FindPattern(TARGET_MODULE, PATTERN_BYTES);
        if (addr) {
            g_PathFindTo = reinterpret_cast<PathFindTo_t>(addr);
            printf("[Pathfinding] Found via pattern: 0x%08X\n", addr);
            return true;
        }
    }

    // 尝试方式二：固定偏移（如果游戏版本已知）
    HMODULE hMod = GetModuleHandleA(TARGET_MODULE);
    if (hMod && PATHFIND_FUNCTION_OFFSET > 0) {
        uintptr_t addr = reinterpret_cast<uintptr_t>(hMod) + PATHFIND_FUNCTION_OFFSET;
        g_PathFindTo = reinterpret_cast<PathFindTo_t>(addr);
        printf("[Pathfinding] Using fixed offset: 0x%08X\n", addr);
        return true;
    }

    printf("[Pathfinding] Function not found! Use CE to find it.\n");
    printf("[Pathfinding] Check the guide in pathfinding.h for details.\n");
    return false;
}

bool MoveTo(int targetX, int targetY, int mapId) {
    if (!g_PathFindTo) {
        printf("[Pathfinding] Function not initialized!\n");
        return false;
    }

    // 需要获取 GameWorld 指针
    DWORD_PTR localPlayer = *reinterpret_cast<DWORD_PTR*>(LOCALPLAYER);
    if (!localPlayer) {
        printf("[Pathfinding] LocalPlayer null\n");
        return false;
    }

    // 调用寻路函数
    // 注意：实际参数数量和格式需要你逆向确定
    // 可能没有 GameWorld 参数，是全局函数
    // 可能参数顺序不同 (y, x) 或 (mapId, x, y)
    int result = g_PathFindTo(
        reinterpret_cast<void*>(localPlayer),  // this 指针
        targetX,
        targetY,
        mapId
    );

    printf("[Pathfinding] MoveTo(%d, %d, %d) = %d\n", targetX, targetY, mapId, result);
    return true;
}

// =============================================================
// 方式二：注入寻路目标坐标
//
// 游戏内部肯定有一个"当前寻路目标"的内存位置。
// 当你按下 G 键时，游戏读取这个坐标，然后开始沿路走过去。
//
// 找到它的方法（CE）：
// 1. 在游戏中，随便点一个地方触发自动寻路
// 2. CE 搜索你的目标X坐标（4字节）
// 3. 取消寻路，换个位置再点，搜索变化的值
// 4. 重复直到找到固定地址
// 5. 这个地址减去 LOCALPLAYER 就是偏移量
// =============================================================

void SetPathTarget(int x, int y, int mapId) {
    // 这些地址需要你用 CE 找到后填入
    // 写入目标坐标，游戏就会自动开始寻路
    // 通常触发方式是:
    //   1. 写入目标坐标到游戏内存
    //   2. 设置一个 "需要寻路" 的标志位为 1
    //   3. 或者直接调用处理函数

    printf("[Pathfinding] Setting path target: (%d, %d) map=%d\n", x, y, mapId);
    printf("[Pathfinding] FILL ME: need actual offsets from CE!\n");
}

void TriggerPathfinding() {
    // 方式一：模拟按 G 键
    // 如果目标坐标已经写入，按 G 触发
    if (g_D3D9.GameWindow) {
        SendMessage(g_D3D9.GameWindow, WM_KEYDOWN, 0x47, 0); // 'G'
        Sleep(50);
        SendMessage(g_D3D9.GameWindow, WM_KEYUP, 0x47, 0);
        printf("[Pathfinding] G-key triggered\n");
    }
}

// =============================================================
// 方式三：封包移动
//
// QQ三国的移动是服务端重演的，真正可靠的移动方式是通过
// 网络封包发送移动指令。
//
// 找到移动封包格式的方法（WPE / WireShark）：
// 1. 用 WPE 或 WireShark 抓包
// 2. 过滤出从本机发往游戏服务器的 UDP/TCP 包
// 3. 在游戏里走一步，看哪个包变了
// 4. 分析封包结构（通常包含：操作码 + X坐标 + Y坐标 + 校验）
// 5. 用 send() 或 WSASend() 发送构造好的封包
// =============================================================

// 发送移动封包
bool SendMovePacket(int targetX, int targetY) {
    printf("[Pathfinding] SendMovePacket: (%d, %d)\n", targetX, targetY);
    printf("[Pathfinding] FILL ME: need packet structure from WPE!\n");
    return false;
}

// 辅助函数：在屏幕上打印当前坐标（方便 CE 定位）
void DebugShowCoords() {
    DWORD x = Overworld::GetPosX();
    DWORD y = Overworld::GetPosY();
    DWORD map = Overworld::GetMapIndex();
    printf("[DEBUG] Current pos: (%d, %d) MapID: %d\n", x, y, map);
}

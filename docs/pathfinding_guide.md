# QQ三国 内置寻路逆向指南

通过调用游戏自身的寻路函数，实现任意地图任意点自动寻路。

---

## 原理

QQ三国内部有一套完整的道路寻路系统，当：
- 你按 **G 键** → 走向当前任务目标
- 你**双击小地图** → 走到点击位置
- 你**点击任务追踪** → 自动走到 NPC 位置

游戏内部都会调用同一个函数：

```cpp
// 大概是这个签名（参数顺序待逆向确定）
int PathFindTo(int targetX, int targetY, int mapId);
```

我们要做的就是把它的地址找到，然后直接调用。

---

## 方法一：CE 追踪法（推荐，最直观）

### 第1步：定位目标坐标内存

1. 启动游戏，打开 Cheat Engine
2. 附加 QQ三国进程（QQSG.exe）
3. 扫描类型选：**Array of bytes**，搜索你不知道的字节模式

但实际上更简单的方法：

### 第2步：找到寻路功能

1. 在游戏里按 **G 键**（自动寻路到任务目标）
2. 记住你当前的任务目标坐标
3. CE 搜索这个坐标值（4字节）
4. 换个任务目标，再搜索新的坐标
5. 直到找到目标坐标的地址

### 第3步：逆向找到函数

找到目标坐标地址后：

1. 右键地址 → **Find out what writes to this address**
2. 回到游戏触发寻路（按 G 或点小地图）
3. CE 会记录一条写操作指令，例如：
   ```
   game.exe+123ABC - mov [esi+48],eax
   ```
4. **Show disassembler** 查看这条指令所在的函数
5. 往上翻到函数开头，记录地址

### 第4步：提取特征码

在函数开头附近选 20-40 字节作为特征码，比如：

```
55 8B EC 83 EC 14 53 56 57
```

不确定的字节用 `??` 替代。

填到 `pathfinding.cpp` 中：

```cpp
static const char* PATTERN_BYTES = "\x55\x8B\xEC\x83\xEC\x14\x53\x56\x57";
static const char* PATTERN_MASK  = "xxxxxxxx";
```

### 第5步：确定参数

观察函数开头的汇编，看它读取了什么寄存器/栈参数。常见模式：

```
// __thiscall 调用
push mapId
push targetY  
push targetX
mov ecx, gameWorldPtr
call PathFindTo

// 或 __stdcall
push mapId
push targetY
push targetX
call PathFindTo
```

根据反汇编结果调整 `PathFindTo_t` 的函数指针定义。

---

## 方法二：特征码交叉引用

QQ三国用 `MoveLap`（道路）来做寻路。游戏启动时把所有道路数据加载到内存。

1. CE 扫描道路数据的特征（`MoveLapMeta` 结构）
2. 找到引用这些数据的代码区
3. 逆向出寻路算法入口

### 已知特征

```cpp
// MoveLapMeta.Type 枚举
#define ROAD_HORIZONTAL 2  // 水平道路
#define ROAD_SLOPE      4  // 斜坡
#define ROAD_VERTICAL   8  // 垂直道路

// 每条道路有:
//   LapId (2字节), Type (4字节), Angle (4字节),
//   StartPoint (8字节), EndPoint (8字节),
//   prevLapId (2字节), nextLapId (2字节)
// = 共 30 字节 左右
```

可以在 CE 里搜 `2 0 0 0`（水平道路的 Type=2）来找道路数据数组，然后交叉引用。

---

## 方法三：Hook G 键处理函数

G 键是"自动寻路到任务目标"的快捷键。Hook 它的处理函数，就能在按 G 时拦截并替换目标坐标。

### 找 G 键处理

1. CE 打开游戏
2. 勾选 **Enable debugger options**
3. 在 **Debug** → **Break on keyboard input**
4. 在游戏里按 G
5. CE 停在处理 G 键的代码处
6. 分析上下文找到 PathFindTo 的调用位置

### 代码框架

```cpp
// Hook G 键消息
LRESULT __stdcall WndProc_h(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN && wParam == 'G') {
        // 在这里改写目标坐标到游戏内存
        SetMyPathTarget();
        // 然后跳过游戏原有的 G 键处理
        return 0;
    }
    return CallWindowProcW((WNDPROC)o_WndProc, hWnd, uMsg, wParam, lParam);
}
```

---

## 方法四：直接调用坐标设置函数

比找 PathFindTo 更简单的是找 **"设置自动寻路目标"** 的函数。

这个函数签名大概是：

```cpp
void SetAutoWalkTarget(int x, int y, int mapId);
```

找到方法：
1. CE 搜目标X坐标 → 改变目标 → 扫描变化 → 找到地址
2. "找出是什么改写了这个地址"
3. 触发寻路，记录修改该地址的指令
4. 那条指令所在的函数就是 SetAutoWalkTarget

调用它之后，游戏会自动开始寻路，无需额外操作。

---

## 常见问题

### Q: 游戏更新后地址变了怎么办？
A: 用特征码扫描（pattern scan），只要代码没重构就能自动找到。

### Q: 调用后角色不动？
A: 
- 检查坐标是否在当前地图范围内
- 检查是否在对话状态中（ConversationState）
- 检查是否在输入状态中（TypingState）
- 地图可能不支持自动寻路（如副本内）

### Q: 怎么确认参数顺序？
A: 在 CE 中看反汇编：

```asm
push 100          ; targetX
push 200          ; targetY
push 1            ; mapId
call 123456       ; PathFindTo
```

看栈上压入的顺序就是你调用时要传的顺序。

---

## 成功后的调用示例

填入偏移/特征码后，你的代码可以这样用：

```cpp
#include "features/pathfinding.h"

// 在 EndScene 或某个按键触发时调用
if (PressedKeys[VK_F1]) {
    // 走到 (1000, 2000) 坐标
    MoveTo(1000, 2000, 461);  // 461 = 洞窟1
}

if (PressedKeys[VK_F2]) {
    // 回到主城
    MoveTo(500, 300, 1);
}
```

或者更高级的用法——任务坐标自动寻路：

```cpp
// 读取任务目标坐标 → 自动寻路
struct QuestGoal {
    int mapId;
    int x, y;
};

void AutoCompleteQuest(QuestGoal goal) {
    if (Overworld::GetMapIndex() != goal.mapId) {
        // 先用传送，否则在当前地图直接走过去
        return;
    }
    MoveTo(goal.x, goal.y, goal.mapId);
}
```

---

**总结：先用 CE 跟着方法一的步骤走，找到函数地址后填入 pathfinding.cpp，剩下的我来帮你搞定。**

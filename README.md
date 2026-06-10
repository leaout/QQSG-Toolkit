# QQSG-Toolkit

QQ三国 辅助开发基础项目。整合自 [QQSG-Hacker](https://github.com/M0rtale/QQSG-Hacker) 的 D3D9 注入/绘制框架与 [qqsg_struct](https://github.com/ziwenhahaha/qqsg_struct) 的完整数据结构定义。

## 项目目标

提供一个开箱即用的 QQ三国 DLL 注入开发框架，让你不用从头折腾 D3D9 Hook、内存读取、数据结构定义这些基础设施，直接上手写功能。

## 功能概览

| 模块 | 说明 |
|------|------|
| **DLL 注入** | DLL劫持注入（msacm32.dll -> msacm32.drv），也支持任意注入器 LoadLibrary |
| **D3D9 Hook** | 通过 CFG Guard 钩子捕获 D3D9 设备，Hook EndScene/BeginScene/Reset/Present |
| **Overlay 绘制** | 纯 D3D9 的文字/方框/线条绘制（不依赖 d3dx9），GDI 渲染后写入纹理 |
| **内存读取** | 模板化的内存读写、指针链解析、VirtualProtect 保护 |
| **数据结构** | 完整的游戏对象结构定义（角色、装备、背包、技能、状态树、UI控件、道路系统等） |
| **自动跑路** | 6个洞窟的 if-else 坐标导航系统（来自 QQSG-Hacker） |
| **内置寻路** | 可替换上述系统的内置寻路框架（通过特征码扫描调用游戏自身寻路函数） |
| **Lua Hook** | Lua 函数钩子工具（需自行提供 lua 开发环境） |
| **API 钩子** | JMP 重定位实现的通用 API 拦截工具 |

## 快速开始

### 编译环境

- Visual Studio 2022
- C++17
- Windows SDK 10.0+（自带 D3D9 头文件）
- Microsoft Detours（已自带 extern/detours/）

### 编译

**方法一（推荐）：双击 build.bat**
以**管理员身份**运行 build.bat，自动查找 VS2022 并编译。

**方法二：Visual Studio**
1. 双击打开 QQSG-Toolkit.sln
2. 工具栏选择：**Release** + **x86**
3. 菜单：生成 -> 生成解决方案

### 注入使用

1. 将编译好的 bin\Release\QQSG-Toolkit.dll 重命名为 **msacm32.drv**
2. 复制到 QQ三国游戏根目录（如 C:\Program Files\腾讯游戏\QQ三国\QQSG\）
3. 启动游戏，DLL 会被自动加载
4. 游戏内按 **Delete 键** 可卸载

## 调整偏移

游戏更新后，用 Cheat Engine 扫描并更新 src/game_data/offsets.h：

```cpp
#define LOCALPLAYER  0x10F9134   // 游戏更新后重新扫描
#define INTYPE       0x10FB3DC   // 同上
```

## 项目结构

```
QQSG-Toolkit/
├── src/
│   ├── dllmain.cpp                       DLL入口点（注入线程启动）
│   ├── d3d9_manager.h/cpp                D3D9 设备捕获+Hook（CFG Guard方式）
│   ├── overlay.h/cpp                     Overlay 绘制逻辑
│   ├── renderer.h/cpp                    纯 D3D9 绘制引擎（GDI->纹理->屏幕）
│   ├── pattern.h                         特征码扫描（内存模式匹配）
│   ├── apihook.h                         API JMP钩子工具
│   ├── apifunctions.h                    钩子回调函数（注释掉的参考实现）
│   ├── memory.h                          内存读写工具
│   ├── base.h                            自旋锁等基础工具
│   ├── game_data.h                       数据结构汇总头文件
│   ├── game_data/
│   │   ├── offsets.h                     全部偏移量
│   │   ├── enums.h                       CtrlState/技能标志/移动状态等枚举
│   │   ├── character.h                   角色/装备/坐标工具函数
│   │   ├── backpack.h                    背包结构（虚表驱动的物品信息读取）
│   │   ├── status.h                      状态树结构（红黑树+状态实例）
│   │   ├── skill.h                       技能数值表
│   │   ├── world.h                       GameWorld/CurMap
│   │   ├── camera.h                      相机结构
│   │   ├── ui.h                          UI控件树（可见性检测）
│   │   └── movelap.h                     道路/传送带结构
│   └── features/
│       ├── auto_runner.h/cpp             自动跑路（6个洞窟坐标查表）
│       ├── pathfinding.h/cpp             内置寻路框架（特征码扫描寻路函数）
│       └── lua_hook.h/cpp                Lua函数钩子工具
├── extern/
│   └── detours/                          Microsoft Detours
├── docs/
│   └── pathfinding_guide.md              内置寻路逆向指南
├── build.bat                             一键编译脚本
├── setup.ps1                             环境检查脚本
└── README.md
```

## 扩展开发

### 1. 添加新功能

在 src/features/ 下新建 .h/.cpp，然后：
- 在 overlay.cpp 中添加绘制代码
- 或者注册键盘快捷键触发功能

### 2. 使用内置寻路系统

参照 docs/pathfinding_guide.md 的 CE 逆向指南，找到游戏寻路函数后填入 pathfinding.cpp：

```cpp
static const char* PATTERN_BYTES = "\x55\x8B\xEC\x83\xEC\x14\x53\x56\x57";
static const char* PATTERN_MASK  = "xxxxxxxx";

// 之后就能任意地图任意点自动寻路
MoveTo(1000, 2000, 461);  // 走到洞窟1的(1000, 2000)
```

### 3. 使用 Lua Hook

在 lua_hook.h 中解除 `#ifdef USE_LUA` 的注释条件，配置好 lua 环境后即可使用。

## 注意事项

- 本项目只作为学习和研究用途
- 使用 DLL 劫持方式注入时，请确保备份原版 msacm32.dll
- 如果编译报错 detours.lib not found，运行 setup.ps1 检查环境
- 游戏版本更新后偏移量会变化，需要用 CE 重新扫描 offsets.h 中的基址
- pathfinding.cpp 和 lua_hook.cpp 为参考实现框架，特征码/偏移需自行逆向填充

## 已知问题与修复

本项目已修复以下从原始整合代码中继承的问题：

| 问题 | 文件 | 修复 |
|------|------|------|
| Equipment 函数未解引用 LOCALPLAYER，读取了错误内存 | character.h | 新增 GetLocalPlayerObject() / GetEquipSlot() 辅助函数 |
| ReadPointerChain 指针链顺序颠倒 | memory.h | 修正为先加偏移再解引用 |
| FreeHook 只清除了 sizeof(pointer) 而非 sizeof(struct) | apihook.h | 修正为 sizeof(hook_t) |
| 自旋锁无互斥语义 | base.h | 重写为 compare_exchange_weak 自旋锁，支持递归进入 |
| DrawBorderBox 边框绘制逻辑错误 | renderer.cpp | 简化为单层边框线绘制 |
| RunHole6 存在永远无法触发的条件分支 | auto_runner.cpp | 修正为 x >= 3900 && x <= 3980 |
| 字体渲染丢失抗锯齿 | renderer.cpp | 用像素亮度值作为 Alpha，保留 GDI 抗锯齿效果 |
| WinIsVisible 使用 find_first_of + index > size() | ui.h | 改用 std::string::npos |
| world.h 中 Camera\* Camera 成员与类型名冲突导致编译错误 | world.h | 重命名成员为 GameCamera |
| UTF-8 编码文件被 MSVC 当做 GBK 解析，吃掉换行符 | .vcxproj | 添加 /utf-8 编译选项 |
| build.bat 相对路径找不到 .sln 文件 | build.bat | 添加 cd /d %~dp0 |
| build.bat 平台名 x86 与项目文件 Win32 不匹配 | build.bat | 统一为 Win32 |
| 未使用的字段和声明 | 多处 | 清理 Sleeper、apifunctions.h 注释化、RunHoleExit 签名简化 |

## 许可证

MIT


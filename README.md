# QQSG-Toolkit

QQ三国 辅助开发基础项目。

基于 [QQSG-Hacker](https://github.com/M0rtale/QQSG-Hacker) 的 D3D9 注入/绘制框架 与 [qqsg_struct](https://github.com/ziwenhahaha/qqsg_struct) 的完整数据结构定义整合而成。

## 功能

- **DLL 注入** — DLL劫持注入方式（msacm32.dll → msacm32.drv）
- **D3D9 Hook** — 通过 CFG 钩子捕获 D3D9 设备，Hook EndScene/BeginScene/Reset/Present
- **Overlay 绘制** — 屏幕文字/方框/线条信息叠加层（装备耐久、坐标、地图等）
- **内存读取** — 角色属性、装备耐久、坐标、状态等
- **数据结构** — 完整的游戏对象结构定义（角色、背包、技能、状态树、UI控件等）
- **自动跑路** — 6个洞窟坐标导航系统
- **Lua Hook** — Lua 函数钩子工具（需自行提供 lua 开发环境）

## 编译环境

- Visual Studio 2022
- C++17
- Windows SDK 10.0+（自带 D3D9 头文件）
- Microsoft Detours（已自带 extern/detours/）

## 快速开始

### 1. 编译

**方法一（推荐）：双击 build.bat**
以**管理员身份**运行 `build.bat`，自动查找 VS2022 并编译。

**方法二：Visual Studio**
1. 双击打开 `QQSG-Toolkit.sln`
2. 工具栏选择：**Release** + **x86**
3. 菜单：生成 → 生成解决方案

### 3. 注入使用

1. 将编译好的 `bin\Release\QQSG-Toolkit.dll` 重命名为 **msacm32.drv**
2. 复制到 QQ三国游戏根目录（如 `C:\Program Files\腾讯游戏\QQ三国\`）
3. 启动游戏，DLL 会被自动加载
4. 游戏内按 **Delete 键** 可卸载

## 调整偏移

游戏更新后，用 CE（Cheat Engine）扫描以下基址并更新 `src/game_data/offsets.h`：

```cpp
#define LOCALPLAYER  0x10F9134   // ← 用 CE 重新扫
#define INTYPE       0x10FB3DC   // ← 用 CE 重新扫
```

## 项目结构

```
QQSG-Toolkit/
├── build.bat                        ← 一键编译脚本
├── setup.ps1                        ← 环境检查脚本
├── QQSG-Toolkit.sln / .vcxproj      ← VS2022 项目文件
├── README.md
├── src/
│   ├── dllmain.cpp                  ← DLL入口点（注入线程启动）
│   ├── d3d9_manager.h/cpp           ← D3D9 设备捕获+钩子
│   ├── overlay.h/cpp                ← Overlay 绘制
│   ├── pattern.h                    ← 特征码扫描
│   ├── apihook.h                    ← API 钩子（JMP Hook）
│   ├── apifunctions.h               ← 钩子回调
│   ├── memory.h                     ← 内存读写工具
│   ├── base.h                       ← 自旋锁等基础工具
│   ├── game_data.h                  ← 数据结构汇总
│   ├── game_data/
│   │   ├── offsets.h                ← 全部偏移量
│   │   ├── enums.h                  ← CtrlState/技能标志等枚举
│   │   ├── character.h              ← 角色/装备/坐标
│   │   ├── backpack.h               ← 背包结构
│   │   ├── status.h                 ← 状态树结构
│   │   ├── skill.h                  ← 技能数值表
│   │   ├── world.h                  ← GameWorld/CurMap
│   │   ├── camera.h                 ← 相机结构
│   │   ├── ui.h                     ← UI控件树
│   │   └── movelap.h                ← 道路/传送带结构
│   └── features/
│       ├── auto_runner.h/cpp        ← 自动跑路（6个洞窟）
│       └── lua_hook.h/cpp           ← Lua钩子工具
└── extern/
    ├── detours/                     ← Microsoft Detours
    └── d3dx9/                       ← 需要自行配置
```

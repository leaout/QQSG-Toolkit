#include "auto_runner.h"
#include <thread>
#include <chrono>

HWND AutoRunner::GameWindow = nullptr;

void AutoRunner::KeyDown(int key) {
    SendMessage(GameWindow, WM_KEYDOWN, key, NULL);
}

void AutoRunner::KeyUp(int key) {
    SendMessage(GameWindow, WM_KEYUP, key, NULL);
}

void AutoRunner::KeyPress(int key) {
    KeyDown(key);
    KeyUp(key);
}

void AutoRunner::KeyPressDelayed(int key, int delayMs) {
    KeyDown(key);
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    KeyUp(key);
}

// ---------- 洞窟导航 ----------
// 以下路径数据来自 QQSG-Hacker Runner.h
// 原理：根据当前坐标 (x,y) 和速度，决定按哪个方向键

bool AutoRunner::RunHole1(int x, int y, int speedX) {
    if (y == 1050 && x > 2050 && x <= 2200) { KeyDown(VK_LEFT); }
    else if (y == 1050 && x <= 2050 && x >= 1930) { KeyPress(VK_UP); }
    else if (y == 820 && x >= 2830 && x <= 2190) { KeyDown(VK_LEFT); }
    else if (y == 860 && x >= 2330 && x <= 2510) { KeyDown(VK_LEFT); }
    else if (y == 920 && x >= 2310 && x <= 2480) { KeyDown(VK_LEFT); }
    else if (y == 970 && x >= 2190 && x <= 2330) { KeyDown(VK_LEFT); }
    else if (y == 900 && x >= 2020 && x <= 2190) { KeyDown(VK_LEFT); }
    else if (y == 980 && x >= 1640 && x <= 2050) { KeyDown(VK_LEFT); if (x <= 1705) KeyPress(VK_UP); }
    else if (y == 910 && x >= 1510 && x <= 1650) { KeyPress(VK_UP); }
    else if (y == 1340 && x > 230 && x <= 340) { KeyDown(VK_LEFT); }
    else if (y == 1340 && x <= 230 && x >= 160) { KeyPress(VK_UP); }
    else if (y == 1280 && x < 130 && x >= 60) { KeyDown(VK_RIGHT); }
    else if (y == 1280 && x >= 130 && x <= 230) { KeyPress(VK_UP); }
    else if (y == 1220 && x <= 300 && x >= 130) { KeyDown(VK_RIGHT); if (x >= 235) KeyPress(VK_UP); }
    else if (y == 1180 && x >= 300 && x <= 440) { KeyDown(VK_RIGHT); if (x >= 385) KeyPress(VK_UP); }
    else if (y == 1120 && x >= 440 && x <= 540) { KeyDown(VK_RIGHT); }
    else if (y == 1120 && x >= 440 && x <= 690) { KeyPress(VK_UP); }
    else if (y == 1040 && x >= 540 && x <= 700) { KeyDown(VK_LEFT); if (x <= 605) KeyPress(VK_UP); }
    else if (y == 980 && x <= 540 && x > 450) { KeyDown(VK_LEFT); }
    else if (y == 980 && x >= 370 && x <= 430) { KeyPress(VK_UP); }
    else if (y == 900 && x >= 260 && x < 410) { KeyDown(VK_RIGHT); }
    else if (y == 900 && x >= 410 && x <= 430) { KeyPress(VK_UP); }
    else if (y == 810 && x >= 90 && x <= 270) { KeyDown(VK_RIGHT); }
    else if (y == 830 && x >= 410 && x <= 580) {
        KeyDown(VK_RIGHT);
        if (speedX <= 200) { if (x >= 568) KeyPress(VK_UP); }
        else { if (x >= 510) KeyPress(VK_UP); }
    }
    else if (y == 860 && x >= 850 && x <= 1610) { KeyDown(VK_LEFT); }
    else if (y == 860 && x >= 640 && x <= 690) { KeyDown(VK_RIGHT); }
    else if (y == 860 && x >= 690 && x <= 850) { KeyPress(VK_UP); }
    else if (y == 780 && x >= 690 && x <= 810) { KeyDown(VK_RIGHT); }
    else if (y == 780 && x >= 810 && x <= 850) { KeyPress(VK_UP); }
    else if (y == 710 && x >= 900 && x <= 960) { KeyDown(VK_LEFT); }
    else if (y == 710 && x >= 810 && x <= 900) { KeyPress(VK_UP); }
    else if (y == 640 && x >= 790 && x <= 900) { KeyDown(VK_LEFT); }
    else if (y == 640 && x >= 750 && x <= 790) { KeyPress(VK_UP); }
    else if (y == 560 && x >= 620 && x <= 800) {
        KeyDown(VK_RIGHT);
        if (speedX <= 200) { if (x >= 763) KeyPress(VK_UP); }
        else { if (x >= 725) KeyPress(VK_UP); }
    }
    else if (y == 470 && x >= 180 && x <= 350) { KeyDown(VK_RIGHT); }
    else if (y == 510 && x >= 330 && x <= 590) { KeyDown(VK_RIGHT); }
    else if (y == 510 && x >= 590 && x <= 650) { KeyPress(VK_UP); }
    else if (y == 440 && x >= 590 && x <= 780) {
        KeyDown(VK_RIGHT);
        if (speedX <= 200) { if (x >= 740) KeyPress(VK_UP); }
        else { if (x >= 700) KeyPress(VK_UP); }
    }
    else if (y == 360 && x >= 790 && x <= 820) { KeyDown(VK_RIGHT); }
    else if (y == 490 && x >= 840 && x <= 1010) { KeyDown(VK_RIGHT); if (x >= 955) KeyPress(VK_UP); }
    else if (y == 430 && x >= 1020 && x <= 1380) { KeyDown(VK_RIGHT); }
    else if (y == 520 && x >= 1390 && x <= 1600) { KeyPress(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 260 && x >= 1340 && x <= 1670) {
        KeyDown(VK_LEFT);
        if (speedX <= 200) { if (x <= 1385) KeyPress(VK_UP); }
        else if (speedX <= 250) { if (x <= 1400) KeyPress(VK_UP); }
        else { if (x <= 1470) KeyPress(VK_UP); }
    }
    else if (y == 250 && x >= 1150 && x <= 1300) {
        KeyDown(VK_LEFT);
        if (speedX <= 200) { if (x <= 1195) KeyPress(VK_UP); }
        else { if (x <= 1245) KeyPress(VK_UP); }
    }
    else if (y == 220 && x >= 890 && x <= 1120) {
        KeyDown(VK_LEFT);
        if (speedX <= 200) { if (x <= 935) KeyPress(VK_UP); }
        else { if (x <= 990) KeyPress(VK_UP); }
    }
    else if (y == 210 && x >= 330 && x <= 860) { KeyDown(VK_LEFT); }
    else if (y == 210 && x >= 140 && x <= 270) { KeyDown(VK_RIGHT); }
    else if (y == 210 && x >= 270 && x <= 330) { KeyPress(VK_UP); return true; }
    return false;
}

bool AutoRunner::RunHole2(int x, int y, int speedX) {
    if (y == 1060 && x >= 2520 && x < 2600) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 1060 && x <= 2690 && x > 2600) { KeyDown(VK_LEFT); KeyDown(VK_UP); }
    else if (y == 1060 && x == 2600) { KeyDown(VK_UP); }
    else if (y == 1110 && x >= 2740 && x <= 2880) {
        KeyDown(VK_LEFT);
        if (speedX <= 200) { if (x <= 2750) KeyPress(VK_UP); }
        else { if (x <= 2760) KeyPress(VK_UP); }
    }
    else if (y == 1110 && x >= 2700 && x < 2740) { KeyDown(VK_RIGHT); }
    else if (y == 1190 && x > 2880 && x <= 3030) { KeyDown(VK_LEFT); }
    else if (y == 1190 && x > 2660 && x < 2700) { KeyDown(VK_RIGHT); }
    else if (y == 1190 && x > 2700 && x < 2880) { KeyPress(VK_UP); }
    else if (y == 1270 && x < 2660 && x <= 2700) { KeyDown(VK_RIGHT); }
    else if (y == 1270 && x >= 2660 && x <= 2700) { KeyPress(VK_UP); }
    else if (y == 950 && x >= 2830 && x <= 3130) { KeyDown(VK_LEFT); }
    else if (y == 980 && x < 1995 && x >= 1640) { KeyDown(VK_RIGHT); }
    else if (y == 980 && x >= 1995 && x <= 2050) { KeyPress(VK_UP); }
    else if (y == 900 && x < 2150 && x >= 2020) { KeyDown(VK_RIGHT); }
    else if (y == 900 && x >= 2150 && x <= 2190) { KeyPress(VK_UP); }
    else if (y == 1050 && x < 2160 && x >= 1930) { KeyDown(VK_RIGHT); }
    else if (y == 1050 && x >= 2160 && x <= 2200) { KeyPress(VK_UP); }
    else if (y == 970 && x < 2280 && x >= 2190) { KeyDown(VK_RIGHT); }
    else if (y == 970 && x >= 2280 && x <= 2330) { KeyPress(VK_UP); }
    else if (y == 920 && x < 2330 && x >= 2310) { KeyDown(VK_RIGHT); }
    else if (y == 920 && x >= 2330 && x <= 2480) { KeyPress(VK_UP); }
    else if (y == 860 && x >= 2330 && x <= 2510) { KeyPress(VK_UP); }
    else if (y == 820 && x < 2280 && x >= 2190) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 820 && x >= 2280 && x <= 2830) {
        KeyDown(VK_LEFT);
        if (y == 820 && x >= 2280 && x <= 2400) KeyDown(VK_UP);
    }
    else if (y == 570 && x < 2535 && x >= 2190) { KeyDown(VK_RIGHT); }
    else if (y == 570 && x >= 2535 && x <= 2620) {
        if (speedX <= 200) { if (x >= 2540) KeyPress(VK_UP); }
        else { if (x >= 2515) KeyPress(VK_UP); }
    }
    else if (y == 500 && x < 2790 && x >= 2570) {
        if (speedX <= 200) { if (x >= 2750) KeyPress(VK_UP); }
        else { if (x >= 2700) KeyPress(VK_UP); }
        KeyDown(VK_RIGHT);
    }
    else if (y == 480 && x < 3080 && x >= 2830) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 480 && x >= 3080 && x <= 3160) { KeyDown(VK_LEFT); KeyDown(VK_UP); }
    else if (y == 230 && x >= 3000 && x <= 3150) {
        KeyDown(VK_LEFT);
        if (speedX <= 200) { if (x <= 3040) KeyPress(VK_UP); }
        else { if (x <= 3070) KeyPress(VK_UP); }
    }
    else if (y == 210 && x >= 2610 && x <= 2960) {
        KeyDown(VK_LEFT);
        if (speedX <= 200) { if (x <= 2660) KeyPress(VK_UP); }
        else { if (x <= 2715) KeyPress(VK_UP); }
    }
    else if ((y == 180 || y == 390 || y == 430) && x >= 1940 && x <= 2620) { KeyDown(VK_LEFT); }
    else if (y == 480 && x >= 1930) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 480 && x <= 1930) { KeyDown(VK_LEFT); KeyDown(VK_UP); }
    else if (y == 210 && x > 1945 && x <= 1990) { KeyDown(VK_LEFT); }
    else if (y == 210 && x >= 1850 && x <= 1900) { KeyDown(VK_RIGHT); }
    else if (y == 210 && x <= 1945 && x >= 1900) { KeyPress(VK_UP); }
    return false;
}

bool AutoRunner::RunHole3(int x, int y, int speedX) {
    if (y == 1060 && x >= 2520 && x < 2600) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 1060 && x <= 2690 && x > 2600) { KeyDown(VK_LEFT); KeyDown(VK_UP); }
    else if (y == 1060 && x == 2600) { KeyDown(VK_UP); }
    else if (y == 1110 && x >= 2740 && x <= 2880) {
        KeyDown(VK_LEFT);
        if (speedX <= 200) { if (x <= 2750) KeyPress(VK_UP); }
        else { if (x <= 2760) KeyPress(VK_UP); }
    }
    else if (y == 1110 && x >= 2700 && x < 2740) { KeyDown(VK_RIGHT); }
    else if (y == 1190 && x > 2880 && x <= 3030) { KeyDown(VK_LEFT); }
    else if (y == 1190 && x > 2660 && x < 2700) { KeyDown(VK_RIGHT); }
    else if (y == 1190 && x > 2700 && x < 2880) { KeyPress(VK_UP); }
    else if (y == 1270 && x < 2660 && x >= 2550) { KeyDown(VK_RIGHT); }
    else if (y == 1270 && x >= 2660 && x <= 2700) { KeyPress(VK_UP); }
    else if (y == 980 && x < 1995 && x >= 1640) { KeyDown(VK_RIGHT); }
    else if (y == 980 && x >= 1995 && x <= 2050) { KeyPress(VK_UP); }
    else if (y == 900 && x < 2150 && x >= 2020) { KeyDown(VK_RIGHT); }
    else if (y == 900 && x >= 2150 && x <= 2190) { KeyPress(VK_UP); }
    else if (y == 1050 && x < 2160 && x >= 1930) { KeyDown(VK_RIGHT); }
    else if (y == 1050 && x >= 2160 && x <= 2200) { KeyPress(VK_UP); }
    else if (y == 970 && x < 2280 && x >= 2190) { KeyDown(VK_RIGHT); }
    else if (y == 970 && x >= 2280 && x <= 2330) { KeyPress(VK_UP); }
    else if (y == 920 && x < 2330 && x >= 2310) { KeyDown(VK_RIGHT); }
    else if (y == 920 && x >= 2330 && x <= 2480) { KeyPress(VK_UP); }
    else if (y == 860 && x >= 2330 && x <= 2510) { KeyPress(VK_UP); }
    else if (y == 820 && x >= 2190 && x <= 2830) { KeyDown(VK_RIGHT); }
    else if (y == 950 && x <= 3090 && x >= 2830) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 950 && x > 3090 && x <= 3150) { KeyDown(VK_LEFT); KeyDown(VK_UP); }
    else if (y == 700 && x <= 3410 && x >= 3000) { KeyDown(VK_RIGHT); }
    else if (y == 700 && x > 3410 && x <= 3540) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 450 && x <= 3370 && x >= 3340) { KeyDown(VK_RIGHT); }
    else if (y == 450 && x > 3370 && x <= 3470) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (x == 3370 && y <= 265) {
        KeyUp(VK_UP);
        KeyPress(VK_UP);
    }
    else if (y == 180 && x <= 3750 && x >= 3290) {
        KeyDown(VK_RIGHT);
        if (y == 180 && x >= 3650 && x <= 3750) KeyDown(VK_RIGHT); KeyDown(VK_UP);
    }
    else if (y == 180 && x > 3750 && x <= 3850) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 450 && x <= 3790 && x >= 3720) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 450 && x > 3790 && x <= 3840) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 760 && x < 3800 && x >= 3740) { KeyDown(VK_RIGHT); }
    else if (y == 760 && x > 3830 && x <= 3880) { KeyDown(VK_LEFT); }
    else if (y == 760 && x >= 3800 && x <= 3830) { KeyPress(VK_UP); }
    else if (y == 910 && x >= 3280 && x <= 3580) { KeyDown(VK_RIGHT); }
    else if (y == 990 && x >= 3550 && x <= 3730) { KeyDown(VK_LEFT); }
    else if (y == 1090 && x >= 3680 && x <= 3880) { KeyDown(VK_LEFT); }
    else if (y == 1140 && x >= 3170 && x <= 3470) { KeyDown(VK_RIGHT); }
    else if (y == 1310 && x >= 3810 && x <= 3980) { KeyDown(VK_LEFT); }
    else if (y == 1690 && x >= 3650 && x <= 3800) { KeyDown(VK_LEFT); }
    else if (y == 1760 && x >= 3050 && x <= 3700) { KeyDown(VK_LEFT); }
    else if (y == 1830 && x >= 2950 && x <= 3100) { KeyDown(VK_LEFT); }
    return false;
}

bool AutoRunner::RunHole4(int x, int y, int speedX) {
    if (y == 1050 && x < 2060 && x >= 1930) { KeyPress(VK_UP); }
    else if (y == 1050 && x >= 2060 && x <= 2200) { KeyDown(VK_LEFT); }
    else if (y == 980 && x < 1680 && x >= 1640) { KeyPress(VK_UP); }
    else if (y == 980 && x >= 1680 && x <= 2050) { KeyDown(VK_LEFT); }
    else if (y == 910 && x < 1620 && x >= 1510) { KeyPress(VK_UP); }
    else if (y == 910 && x >= 1620 && x <= 1650) { KeyDown(VK_LEFT); }
    else if (y == 860 && x >= 1150 && x <= 1610) { KeyDown(VK_LEFT); }
    else if (y == 860 && x < 1150 && x >= 640) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 1150 && x >= 920 && x <= 1250) { KeyDown(VK_LEFT); }
    else if (y == 1240 && x >= 740 && x <= 940) { KeyDown(VK_LEFT); }
    else if (y == 1310 && x >= 630 && x < 675) { KeyDown(VK_RIGHT); }
    else if (y == 1310 && x > 740 && x <= 800) { KeyDown(VK_LEFT); }
    else if (y == 1310 && x <= 740 && x >= 675) { KeyPress(VK_UP); }
    else if (y == 820 && x >= 2830 && x <= 2190) { KeyDown(VK_LEFT); }
    else if (y == 860 && x >= 2330 && x <= 2510) { KeyDown(VK_LEFT); }
    else if (y == 920 && x >= 2310 && x <= 2480) { KeyDown(VK_LEFT); }
    else if (y == 970 && x >= 2190 && x <= 2330) { KeyDown(VK_LEFT); }
    else if (y == 900 && x >= 2020 && x <= 2190) { KeyDown(VK_LEFT); }
    else if (y == 1590 && x >= 360 && x <= 640) { KeyDown(VK_RIGHT); }
    return false;
}

bool AutoRunner::RunHole5(int x, int y, int speedX) {
    if (y == 980 && x < 1995 && x >= 1640) { KeyDown(VK_RIGHT); }
    else if (y == 980 && x >= 1995 && x <= 2050) { KeyPress(VK_UP); }
    else if (y == 900 && x < 2150 && x >= 2020) { KeyDown(VK_RIGHT); }
    else if (y == 900 && x >= 2150 && x <= 2190) { KeyPress(VK_UP); }
    else if (y == 1050 && x < 2160 && x >= 1930) { KeyDown(VK_RIGHT); }
    else if (y == 1050 && x >= 2160 && x <= 2200) { KeyPress(VK_UP); }
    else if (y == 970 && x < 2280 && x >= 2190) { KeyDown(VK_RIGHT); }
    else if (y == 970 && x >= 2280 && x <= 2330) { KeyPress(VK_UP); }
    else if (y == 920 && x < 2330 && x >= 2310) { KeyDown(VK_RIGHT); }
    else if (y == 920 && x >= 2330 && x <= 2480) { KeyPress(VK_UP); }
    else if (y == 860 && x >= 2330 && x <= 2510) { KeyPress(VK_UP); }
    else if (y == 820 && x < 2600 && x >= 2190) { KeyDown(VK_RIGHT); }
    else if (y == 820 && x >= 2600 && x <= 2830) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 1060 && x >= 2520 && x <= 2690) { KeyDown(VK_LEFT); }
    else if (y == 1120 && x >= 2330 && x <= 2510) { KeyDown(VK_LEFT); }
    else if (y == 1310 && x >= 2070 && x <= 2360) {
        if (speedX <= 200) { if (x <= 2105) KeyPress(VK_UP); }
        else { if (x <= 2170) KeyPress(VK_UP); }
        KeyDown(VK_LEFT);
    }
    else if (y == 1310 && x >= 1980 && x <= 2030) { KeyDown(VK_LEFT); }
    else if (y == 1400 && x >= 1690 && x <= 1960) { KeyDown(VK_LEFT); }
    else if (y == 1480 && x >= 1470 && x <= 1730) { KeyDown(VK_LEFT); }
    else if (y == 1560 && x >= 1390 && x <= 1550) { KeyDown(VK_LEFT); }
    else if (y == 1640 && x >= 1270 && x <= 1440) { KeyDown(VK_RIGHT); }
    else if (y == 1720 && x >= 1390 && x <= 1540) { KeyDown(VK_RIGHT); }
    else if (y == 1800 && x >= 1470 && x <= 1610) { KeyDown(VK_RIGHT); }
    else if ((y == 1930 || y == 1950) && x >= 1480 && x <= 1700) { KeyDown(VK_LEFT); KeyPress(VK_UP); }
    else if (y == 1950 && x < 1480 && x >= 1450) { KeyDown(VK_RIGHT); KeyPress(VK_UP); }
    else if (y == 1930 && x >= 1840 && x <= 2120) {
        KeyDown(VK_RIGHT);
        if (speedX <= 200) { if (x >= 2080) KeyPress(VK_UP); }
        else { if (x >= 2060) KeyPress(VK_UP); }
    }
    else if (y == 1990 && x > 2110 && x <= 2200) { KeyDown(VK_LEFT); }
    else if (y == 1990 && x >= 2180 && x <= 2110) { KeyPress(VK_UP); }
    // ... remaining path points
    else if (y == 1110 && x >= 2740 && x <= 2880) {
        KeyDown(VK_LEFT);
        if (speedX <= 200) { if (x <= 2750) KeyPress(VK_UP); }
        else { if (x <= 2760) KeyPress(VK_UP); }
    }
    else if (y == 1110 && x >= 2700 && x < 2740) { KeyDown(VK_RIGHT); }
    return false;
}

bool AutoRunner::RunHole6(int x, int y, int speedX) {
    if (y == 980 && x < 1995 && x >= 1640) { KeyDown(VK_RIGHT); }
    else if (y == 980 && x >= 1995 && x <= 2050) { KeyPress(VK_UP); }
    else if (y == 900 && x < 2150 && x >= 2020) { KeyDown(VK_RIGHT); }
    else if (y == 900 && x >= 2150 && x <= 2190) { KeyPress(VK_UP); }
    else if (y == 1050 && x < 2160 && x >= 1930) { KeyDown(VK_RIGHT); }
    else if (y == 1050 && x >= 2160 && x <= 2200) { KeyPress(VK_UP); }
    else if (y == 970 && x < 2280 && x >= 2190) { KeyDown(VK_RIGHT); }
    else if (y == 970 && x >= 2280 && x <= 2330) { KeyPress(VK_UP); }
    else if (y == 920 && x < 2330 && x >= 2310) { KeyDown(VK_RIGHT); }
    else if (y == 920 && x >= 2330 && x <= 2480) { KeyPress(VK_UP); }
    else if (y == 860 && x >= 2330 && x <= 2510) { KeyPress(VK_UP); }
    else if (y == 820 && x >= 2190 && x <= 2830) { KeyDown(VK_RIGHT); }
    else if (y == 950 && x >= 2830 && x <= 3090) { KeyDown(VK_RIGHT); }
    else if (y == 950 && x > 3090 && x <= 3140) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 700 && x <= 3290 && x >= 3000) { KeyDown(VK_RIGHT); }
    else if (y == 700 && x <= 3550 && x > 3290) { KeyDown(VK_DOWN); KeyDown(VK_RIGHT); }
    else if ((y == 990 || y == 1090) && x >= 3490 && x < 3680) { KeyDown(VK_RIGHT); }
    else if (y == 1090 && x >= 3680 && x <= 3880) { KeyDown(VK_RIGHT); }
    else if ((y == 3970 || y == 3980 || y == 0) && x >= 3940 && x <= 3980) { KeyPress(VK_UP); }
    else if (y == 1310 && x >= 3940 && x <= 3980) { KeyDown(VK_LEFT); }
    else if (y == 1310 && x < 3940 && x >= 3840) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 1580 && x > 3890) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 1580 && x >= 3840 && x <= 3890) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 1860 && x >= 3850 && x <= 3900) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 1860 && x > 3900 && x <= 3980) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 2180 && x > 3600) { KeyDown(VK_LEFT); }
    else if (y == 2180 && x >= 3390 && x <= 3520) { KeyDown(VK_RIGHT); }
    else if (y == 2180 && x <= 3600 && x > 3520) { KeyPress(VK_UP); }
    else if (y == 2100 && x < 3590 && x > 3500) { KeyDown(VK_RIGHT); }
    else if (y == 2100 && x >= 3600 && x <= 3650) { KeyDown(VK_LEFT); }
    else if (y == 2100 && x >= 3590 && x <= 3600) { KeyPress(VK_UP); }
    else if (y == 2030 && x > 3610 && x <= 3730) { KeyDown(VK_LEFT); }
    else if (y == 2030 && x >= 3560 && x < 3580) { KeyDown(VK_RIGHT); }
    else if (y == 2030 && x <= 3610 && x >= 3580) { KeyPress(VK_UP); }
    else if (y == 1960 && x > 3620 && x <= 3690) { KeyDown(VK_LEFT); }
    else if (y == 1960 && x < 3575 && x >= 3530) { KeyDown(VK_RIGHT); }
    else if (y == 1960 && x >= 3575 && x <= 3620) { KeyPress(VK_UP); }
    else if ((y == 1690 || y == 1760) && x > 3130 && x < 3800) { KeyDown(VK_LEFT); }
    else if (y == 1760 && x <= 3120 && x > 3060) { KeyDown(VK_RIGHT); }
    else if (y == 1760 && x >= 3120 && x <= 3130) { KeyPress(VK_UP); }
    else if (y == 1680 && x < 3100 && x >= 2930) { KeyDown(VK_RIGHT); }
    else if (y == 1680 && x > 3140 && x <= 3180) { KeyDown(VK_LEFT); }
    else if (y == 1680 && x >= 3100 && x <= 3140) { KeyPress(VK_UP); }
    else if (y == 1600 && x >= 3050 && x <= 3190) { KeyPress(VK_UP); }
    else if (y == 1530 && x >= 3000 && x <= 3130) { KeyDown(VK_RIGHT); }
    else if (y == 1530 && x > 3130 && x <= 3180) { KeyPress(VK_UP); }
    else if (y == 1450 && x >= 3150 && x < 3220) { KeyDown(VK_RIGHT); }
    else if (y == 1450 && x >= 3220 && x <= 3330) { KeyPress(VK_UP); }
    else if (y == 1380 && x >= 3220 && x < 3330) { KeyDown(VK_RIGHT); }
    else if (y == 1380 && x >= 3330 && x <= 3420) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 1140 && x < 3380 && x >= 3210) { KeyDown(VK_RIGHT); }
    else if (y == 1140 && x <= 3470 && x >= 3380) { KeyDown(VK_RIGHT); KeyDown(VK_UP); }
    else if (y == 910 && x >= 3300 && x <= 3620) { KeyDown(VK_RIGHT); }
    return false;
}

bool AutoRunner::RunGeneralPos(int x, int y, int speedX) {
    if (x > 980 && y == 2180) { KeyDown(VK_LEFT); }
    else if (y == 2180 && x < 980 && x > 840) { KeyPress(VK_UP); }
    else if (y == 2180 && x <= 840 && x >= 630) { KeyDown(VK_RIGHT); }
    else if (y == 2100 && x > 980 && x <= 1110) { KeyDown(VK_LEFT); }
    else if (y == 2100 && x <= 980) { KeyPress(VK_UP); }
    else if (y == 2020 && x > 920) { KeyDown(VK_LEFT); }
    else if (y == 2020 && x <= 920) { KeyPress(VK_UP); }
    else if (y == 1940 && x < 830) { KeyDown(VK_RIGHT); }
    else if (y == 1940 && x >= 830) { KeyPress(VK_UP); }
    else if (y == 1870 && x > 960) { KeyDown(VK_LEFT); }
    else if (y == 1870 && x <= 960) { KeyPress(VK_UP); }
    else if (y == 1790 && x <= 960 && x >= 780) {
        KeyDown(VK_LEFT);
        if (speedX >= 760) { KeyPress(VK_UP); }
        else {
            if (x <= 860) KeyPress(VK_UP);
            if (x <= 810) KeyPress(VK_UP);
        }
    }
    else if ((y == 1550 || y == 1590 || y == 1660 || y == 1730 || y == 1810 || y == 1880) && x <= 360) { KeyDown(VK_RIGHT); }
    else if (y == 1960 && x < 390) { KeyDown(VK_RIGHT); }
    else if (y == 1960 && x >= 390 && x <= 530) { KeyPress(VK_UP); }
    else if (y == 2040 && x < 300) { KeyDown(VK_RIGHT); }
    else if (y == 2040 && x >= 300) { KeyPress(VK_UP); }
    else if (y == 2110 && x < 260) { KeyDown(VK_RIGHT); }
    else if (y == 2110 && x >= 260) { KeyPress(VK_UP); }
    else if (y == 2180 && x < 160) { KeyDown(VK_RIGHT); }
    else if (y == 2180 && x > 320 && x <= 520) { KeyDown(VK_LEFT); }
    else if (y == 2180 && x <= 320 && x >= 160) { KeyPress(VK_UP); }
    else if (y == 1880 && x < 500) { KeyDown(VK_RIGHT); }
    else if (y == 1880 && x >= 500 && x <= 2110) { KeyPress(VK_UP); }
    else if (y == 1800 && x < 630) { KeyDown(VK_RIGHT); }
    else if (y == 1800 && x >= 630 && x < 1440) { KeyPress(VK_UP); }
    else if (y == 1720 && x < 770) { KeyDown(VK_RIGHT); }
    else if (y == 1720 && x >= 770 && x < 1380) { KeyPress(VK_UP); }
    else if (y == 1640 && x < 960) { KeyDown(VK_RIGHT); }
    else if (y == 1640 && x > 920 && x < 1270) { KeyPress(VK_UP); }
    else if (y == 1560 && x < 1000) { KeyDown(VK_RIGHT); }
    else if (y == 1560 && x >= 1000 && x <= 1070) { KeyPress(VK_UP); }
    else if (y == 1480 && x < 1145) { KeyDown(VK_RIGHT); }
    else if (y == 1480 && x >= 1145 && x <= 1160) { KeyPress(VK_UP); }
    else if (y == 1410 && x < 1290) { KeyDown(VK_RIGHT); }
    else if (y == 1410 && x >= 1290 && x <= 1320) { KeyPress(VK_UP); }
    else if (y == 1330 && x < 1400) { KeyDown(VK_RIGHT); }
    else if (y == 1330 && x >= 1400 && x <= 1440) { KeyPress(VK_UP); }
    else if (y == 1260 && x < 1470) { KeyDown(VK_RIGHT); }
    else if (y == 1260 && x >= 1470 && x <= 1540) { KeyPress(VK_UP); }
    else if (y == 1190 && x < 1830) { KeyDown(VK_RIGHT); }
    else if (y == 1190 && x >= 1830 && x <= 1870) { KeyPress(VK_UP); }
    else if (y == 1110 && x < 1930) { KeyDown(VK_RIGHT); }
    else if (y == 1110 && x >= 1930 && x < 1980) { KeyPress(VK_UP); }
    return false;
}

bool AutoRunner::RunHoleExit(DWORD mapid, DWORD currentstate, DWORD x, DWORD y) {
    if (x <= 1956) {
        KeyDown(VK_RIGHT);
    } else if (x >= 1956 && currentstate == 0) {
        KeyUp(VK_RIGHT);
        KeyPress(0x47); // G key - interact
    } else if (mapid == 462 && currentstate == 1) {
        KeyPress(VK_RETURN);
    } else if (mapid == 462 && currentstate == 2) {
        KeyPress(VK_RETURN);
    } else if (mapid == 462 && currentstate == 14) {
        KeyPress(VK_RETURN);
    } else if (mapid == 462 && currentstate == 21) {
        // Dialog selection using screen pixel color detection
        HDC hdc = GetDC(GameWindow);
        COLORREF A0 = GetPixel(hdc, 316, 292);
        COLORREF B0 = GetPixel(hdc, 314, 316);
        COLORREF C0 = GetPixel(hdc, 316, 349);
        COLORREF D0 = GetPixel(hdc, 316, 379);

        auto isDefault = [](COLORREF c) { return GetRValue(c) == 0 && GetGValue(c) == 25 && GetBValue(c) == 35; };

        if (!isDefault(A0)) { SendMessage(GameWindow, WM_MOUSEMOVE, 0, MAKELPARAM(316, 292)); KeyPress(VK_LBUTTON); }
        else if (!isDefault(B0)) { SendMessage(GameWindow, WM_MOUSEMOVE, 0, MAKELPARAM(314, 316)); KeyPress(VK_LBUTTON); }
        else if (!isDefault(C0)) { SendMessage(GameWindow, WM_MOUSEMOVE, 0, MAKELPARAM(316, 349)); KeyPress(VK_LBUTTON); }
        else if (!isDefault(D0)) { SendMessage(GameWindow, WM_MOUSEMOVE, 0, MAKELPARAM(316, 379)); KeyPress(VK_LBUTTON); }
        else { SendMessage(GameWindow, WM_MOUSEMOVE, 0, MAKELPARAM(316, 379)); KeyPress(VK_LBUTTON); }

        ReleaseDC(GameWindow, hdc);
    }
    return false;
}

bool AutoRunner::RunOutside(DWORD mapid, DWORD currentstate, DWORD x, DWORD y) {
    static bool goDownOne = false;

    if (y >= 682) {
        if (currentstate == 0)
            KeyPress(0x47); // G
        else if (currentstate == 2)
            KeyPress(VK_RETURN);
        else if (currentstate == 1 && !goDownOne) {
            KeyPress(VK_DOWN);
            goDownOne = true;
        } else if (currentstate == 1 && goDownOne) {
            KeyPress(VK_RETURN);
            goDownOne = false;
        }
    }
    return false;
}

bool AutoRunner::ClearType(BOOL intype) {
    if (intype)
        KeyPress(VK_RETURN);
    return false;
}

void AutoRunner::Execute(DWORD worldIndex, DWORD x, DWORD y, DWORD speed) {
    switch (worldIndex) {
    case 461: // HOLE1
    case 458:
    case 457:
    case 459:
    case 460:
    case 456:
    case 462: // HOLEEND
    case 28:  // ENTRYYANNIAODU
        break;
    default:
        return;
    }

    RunGeneralPos(x, y, speed);

    if (worldIndex == 461) RunHole1(x, y, speed);
    else if (worldIndex == 458) RunHole2(x, y, speed);
    else if (worldIndex == 457) RunHole3(x, y, speed);
    else if (worldIndex == 460) RunHole4(x, y, speed);
    else if (worldIndex == 459) RunHole5(x, y, speed);
    else if (worldIndex == 456) RunHole6(x, y, speed);
    else if (worldIndex == 462) RunHoleExit(worldIndex, 0, x, y);
}

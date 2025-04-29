#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

void HideConsole() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);  // 隐藏控制台窗口
}

// 获取 DPI 缩放系数
float GetDPI() {
    HDC hdc = GetDC(NULL);
    int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);  // 水平 DPI
    ReleaseDC(NULL, hdc);
    return dpiX / 96.0f;  // 默认 DPI 为 96
}

// 模拟点击按钮 (btn_inst)
void ClickInstallButton(HWND hwnd) {
    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        int left = rect.left;
        int top = rect.top;

        // 获取当前屏幕的 DPI 缩放系数
        float dpiScale = GetDPI();

        // 将窗口置顶（避免点击无效）
        SetForegroundWindow(hwnd);
        Sleep(500);  // 等待界面稳定

        // 根据 XML 推算按钮位置
        // 容器位置及按钮尺寸（居中按钮）
        int containerTop = top + 212;  // main_interactive 位置的顶部偏移
        int button_x = left + (278 + 60) * dpiScale;  // 居中按钮的x坐标
        int button_y = containerTop + (16 + 32) * dpiScale;  // 按钮的y坐标

        // 模拟鼠标移动并点击
        SetCursorPos(button_x, button_y);
        Sleep(50);  // 稍等一会，模拟点击自然
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        Sleep(50);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }
}

// 查找火绒安装主窗口
HWND FindHuorongWindow() {
    HWND hwnd = NULL;
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        char className[256];
        GetClassNameA(hwnd, className, sizeof(className));

        char windowText[256];
        GetWindowTextA(hwnd, windowText, sizeof(windowText));

        // 自绘窗口类名或窗口标题包含这些字段
        if (strstr(className, "HUORONGINSTALLERWND") || strstr(windowText, "火绒安全")) {
            *((HWND*)lParam) = hwnd;
            return FALSE;  // 找到了，终止枚举
        }
        return TRUE;
        }, (LPARAM)&hwnd);

    return hwnd;
}

int main() {
    HideConsole();  // 隐藏控制台窗口

    std::srand((unsigned int)std::time(nullptr));  // 初始化随机数种子

    HWND hwnd = NULL;
    for (int i = 0; i < 20; i++) {  // 最多等待 10 秒
        hwnd = FindHuorongWindow();
        if (hwnd) {
            ClickInstallButton(hwnd);
            return 0;
        }
        Sleep(500);
    }

    return 1;
}

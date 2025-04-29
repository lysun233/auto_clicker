#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

void HideConsole() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
}

float GetDPI() {
    HDC hdc = GetDC(NULL);
    int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    ReleaseDC(NULL, hdc);
    return dpiX / 96.0f;
}

void ClickInstallButton(HWND hwnd) {
    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        int left = rect.left;
        int top = rect.top;

        float dpiScale = GetDPI();

        SetForegroundWindow(hwnd);
        Sleep(300);

        int button_x = left + static_cast<int>((290 + (510 - 290) / 2) * dpiScale);
        int button_y = top + static_cast<int>((315 + (350 - 315) / 2) * dpiScale);

        SetCursorPos(button_x, button_y);
        Sleep(50);
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        Sleep(50);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }
}

HWND FindSunloginWindow() {
    HWND hwnd = NULL;
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        char className[256] = { 0 };
        char windowText[256] = { 0 };

        GetClassNameA(hwnd, className, sizeof(className));
        GetWindowTextA(hwnd, windowText, sizeof(windowText));

        if (!IsWindowVisible(hwnd)) {
            return TRUE;
        }

        if (strstr(className, "OrayUI") || strstr(windowText, "向日葵") || strstr(windowText, "Sunlogin")) {
            *((HWND*)lParam) = hwnd;
            return FALSE;
        }
        return TRUE;
        }, (LPARAM)&hwnd);

    return hwnd;
}

int main() {
    HideConsole();

    std::srand((unsigned int)std::time(nullptr));

    HWND hwnd = NULL;
    for (int i = 0; i < 20; i++) {
        hwnd = FindSunloginWindow();
        if (hwnd) {
            ClickInstallButton(hwnd);
            return 0;
        }
        Sleep(500);
    }

    return 1;
}

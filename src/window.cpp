//
// Created by MatexPL on 2022-06-01.
//

#include <iostream>
#include <windows.h>

namespace Window {
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    bool Create(HWND* lpHwnd) {
        // Register the window class.
        const char CLASS_NAME[] = "Sample Window Class";
        const HINSTANCE hInstance = (HINSTANCE) GetModuleHandle(nullptr);

        WNDCLASS wc = {};

        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = CLASS_NAME;

        RegisterClass(&wc);

        // Create the window.

        HWND hwnd = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                CLASS_NAME,
                "Learn to Program Windows",
                (WS_OVERLAPPED & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME),

                // Size and position
                CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,

                nullptr,       // Parent window
                nullptr,       // Menu
                hInstance,  // Instance handle
                nullptr        // Additional application data
        );

        *lpHwnd = hwnd;

        if (hwnd == nullptr) {
            return 0;
        }

        ShowWindow(hwnd, 1);
        // Run the message loop.

        MSG msg = {};
        while (GetMessage(&msg, nullptr, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return 0;
    }

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            case WM_PAINT:
                //The window is not resizable... so I guess I can leave this empty?
                return 0;

            case WM_LBUTTONDOWN:
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                Rectangle(hdc, 10, 10, 100, 100);
                return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

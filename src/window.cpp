//
// Created by MatexPL on 2022-06-01.
//

#ifndef UNICODE
#define UNICODE
#endif

#include "iostream"
#include "rendererwoo.cpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            CLASS_NAME,
            L"Learn to Program Windows",
            (WS_OVERLAPPED & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME),

            // Size and position
            CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,

            NULL,       // Parent window
            NULL,       // Menu
            hInstance,  // Instance handle
            NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    // Run the message loop.

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
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

        case WM_PAINT: {
            //The window is not resizable... so I guess I can leave this empty?
        }
        case WM_MBUTTONDOWN: {
            std::cout << "Click" << std::endl;
            Renderer::Draw();
        }
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
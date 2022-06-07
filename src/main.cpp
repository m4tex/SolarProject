#include <windows.h>
#include <chrono>
#include <string>
#include "../headers/engine_structs.h"
#include "../headers/engine_globals.h"
#include "../headers/engine_methods.h"

RECT clientArea;

Engine::SolarObject sword("Sword");

void StartWindow(HINSTANCE hinstance);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    sword.mesh.LoadFromFile("../resources/models/cube.obj");
    sword.position = { 0, 0, 5 };
    Engine::AddToRenderQueue(sword);
    StartWindow(hInstance);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HBRUSH bgBrush = CreateSolidBrush(RGB(0, 0, 0));

void StartWindow(HINSTANCE hinstance) {
    // Register the window class.
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = bgBrush;

    RegisterClass(&wc);

    // Create the window.

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            CLASS_NAME,
            "SolarEngine",
            WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME &  ~WS_MAXIMIZEBOX,

            // Size and position
            (screenWidth / 2) - 263, (screenHeight / 2) - 263, wnd_w, wnd_h + 31,

            nullptr,       // Parent window
            nullptr,       // Menu
            hinstance,  // Instance handle
            nullptr        // Additional application data
    );

    if (hwnd == nullptr) {
        return;
    }

    ShowWindow(hwnd, 1);
    // Run the message loop.

    GetClientRect(hwnd, &clientArea);

    MSG msg;

    while(true){
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
            break;

        PAINTSTRUCT ps;
        HDC deviceCtx = BeginPaint(hwnd, &ps);
        Engine::Render(deviceCtx);
        EndPaint(hwnd, &ps);
        SetWindowText(hwnd, std::to_string(Engine::FPS).c_str());
    }

    return;
}

LRESULT CALLBACK WindowProc(HWND windowHandler, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            return 0;
    }
    return DefWindowProc(windowHandler, uMsg, wParam, lParam);
}
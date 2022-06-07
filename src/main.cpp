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
    sword.mesh.LoadFromFile("../resources/models/sword.obj");
    sword.position = { 0, -1.5, 5 };
    sword.rotation = { 45, 45, 0 };
    Engine::AddToRenderQueue(sword);
    StartWindow(hInstance);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HBRUSH bgBrush = CreateSolidBrush(RGB(0, 0, 0));

const std::string windowTitle = "Solar Engine";

void StartWindow(HINSTANCE hinstance) {
    // Register the window class.
    const char CLASS_NAME[] = "SolarEngine Window Class";

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
            windowTitle.c_str(),
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

    auto t_old = std::chrono::high_resolution_clock::now();
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

        //FPS counter
        auto t_new = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = t_new - t_old;
        SetWindowText(hwnd, (windowTitle + " (FPS: " + std::to_string((int)(1/elapsed.count())) + ")").c_str());
        t_old = t_new;
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
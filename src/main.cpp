#include <windows.h>
#include <chrono>
#include <string>
#include "../headers/engine_structs.h"
#include "../headers/engine_globals.h"
#include "../headers/engine_methods.h"

Engine::Mesh model;

void StartWindow(HINSTANCE hinstance);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    model.LoadFromFile("../resources/models/sword.obj");
    StartWindow(hInstance);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void StartWindow(HINSTANCE hinstance) {
    // Register the window class.
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hinstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            CLASS_NAME,
            "SolarEngine",
            WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,

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

    GetClientRect(hwnd, &Engine::clientArea);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return;
}

void DrawFrame(HDC deviceCtx);

void EngineTick(HWND windowHandler);

LRESULT CALLBACK WindowProc(HWND windowHandler, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC deviceCtx = BeginPaint(windowHandler, &ps);
            DrawFrame(deviceCtx);
            EndPaint(windowHandler, &ps);
            EngineTick(windowHandler);
            return 0;
    }
    return DefWindowProc(windowHandler, uMsg, wParam, lParam);
}

HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
HPEN whitePen = CreatePen(0, 1, RGB(255, 255, 255));

int frame = 0; //int is big enough for ~50 days with 1000fps

void DrawFrame(HDC deviceCtx) {
    FillRect(deviceCtx, &Engine::clientArea, blackBrush);
    SelectObject(deviceCtx, whitePen);

    Engine::DrawMesh(model, deviceCtx);
    frame++;

    //Debug
    //DrawTriangle3DSpace({0, 0, 1, 0, 1, .5, 1, 0, 0}, position, deviceCtx);
}

auto t_start = std::chrono::high_resolution_clock::now();
// the work...
auto t_end = std::chrono::high_resolution_clock::now();

void EngineTick(HWND windowHandler) {
    SetWindowTextA(windowHandler, std::to_string(GetMessageTime()).c_str());
}
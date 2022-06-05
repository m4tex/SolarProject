#include <windows.h>
#include <vector>
#include <string>
#include <chrono>

#define wnd_h 500
#define wnd_w 500

struct Vector3 {
    float x, y, z;
};

struct Triangle {
    Vector3 vertices[3];
};

void StartWindow(HINSTANCE hinstance);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    StartWindow(hInstance);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

RECT clientArea;
RECT windowArea;

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
            WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,

            // Size and position
            (screenWidth/2)-263, (screenHeight/2)-263, wnd_w, wnd_h+31,

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
    GetWindowRect(hwnd, &windowArea);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return;
}

void DrawFrame(HDC deviceCtx);

void EngineTick();

Triangle ProjectTriangleToScreen(Triangle triangle, Vector3 pos);

LRESULT CALLBACK WindowProc(HWND windowHandler, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC deviceCtx = BeginPaint(windowHandler, &ps);
            DrawFrame(deviceCtx);
            EngineTick();
            EndPaint(windowHandler, &ps);
            return 0;
    }
    return DefWindowProc(windowHandler, uMsg, wParam, lParam);
}

HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
HPEN whitePen = CreatePen(0, 1, RGB(255, 255, 255));

Vector3 position = {0, 0, 0};
Vector3 angularVelocity = {1, 0, 0};
Vector3 velocity = {1, 1, 0};

const Triangle triangle = {150, 0, 1, 225, 300, 1, 400, 500, 1};

void DrawFrame(HDC deviceCtx) {
    FillRect(deviceCtx, &clientArea, blackBrush);
    Triangle projected = ProjectTriangleToScreen(triangle, position);

    SelectObject(deviceCtx, whitePen);

    RECT rect = {100, 100, 300, 300};

    MoveToEx(deviceCtx, projected.vertices[0].x, clientArea.bottom - projected.vertices[0].y, nullptr);
    LineTo(deviceCtx, projected.vertices[1].x, clientArea.bottom - projected.vertices[1].y);
    MoveToEx(deviceCtx, projected.vertices[1].x, clientArea.bottom - projected.vertices[1].y, nullptr);
    LineTo(deviceCtx, projected.vertices[2].x, clientArea.bottom - projected.vertices[2].y);
    MoveToEx(deviceCtx, projected.vertices[2].x, clientArea.bottom - projected.vertices[2].y, nullptr);
    LineTo(deviceCtx, projected.vertices[0].x, clientArea.bottom - projected.vertices[0].y);
}

Triangle ProjectTriangleToScreen(Triangle triangle, Vector3 pos) {
    return triangle;
}

void EngineTick() {

}
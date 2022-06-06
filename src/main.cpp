#include <windows.h>
#include <chrono>
#include <cmath>
#include <string>
#include <vector>

#define wnd_h 500
#define wnd_w 500

struct Vector3 {
    float x, y, z;

    Vector3 operator*(float amount) {
        return {x * amount, y * amount, z * amount};
    }

    Vector3 operator*(Vector3 vec) {
        return {x * vec.x, y * vec.y, z * vec.z};
    }

    void operator*=(Vector3 vec) {
        x *= vec.x;
        y *= vec.y;
        z *= vec.z;
    }

    void operator+=(float amount) {
        x += amount;
        y += amount;
        z += amount;
    }

    void operator+=(Vector3 vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;
    }

    void operator/=(float amount) {
        x /= amount;
        y /= amount;
        z /= amount;
    }
};

struct Triangle {
    Vector3 vertices[3];
};

struct Mesh {
    std::vector<Triangle> tris;
};

struct Matrix4x4 {
    float m[4][4] = {0};
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

void DrawMesh(Mesh mesh, HDC deviceCtx);

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

Vector3 camPos;
Vector3 position = {-.5, -.5, 1};
Vector3 scale = {1, 1, 1};
Vector3 angularVelocity = {1, 0, 0};
Vector3 velocity = {1, 1, 0};

//#region cube
const Mesh dummy_cube = {
        .tris {
                // SOUTH
                {0, 0, 0, 0, 1, 0, 1, 1, 0},
                {0, 0, 0, 1, 1, 0, 1, 0, 0},

                // EAST
                {1, 0, 0, 1, 1, 0, 1, 1, 1},
                {1, 0, 0, 1, 1, 1, 1, 0, 1},

                // NORTH
                {1, 0, 1, 1, 1, 1, 0, 1, 1},
                {1, 0, 1, 0, 1, 1, 0, 0, 1},

                // WEST
                {0, 0, 1, 0, 1, 1, 0, 1, 0},
                {0, 0, 1, 0, 1, 0, 0, 0, 0},

                // TOP
                {0, 1, 0, 0, 1, 1, 1, 1, 1},
                {0, 1, 0, 1, 1, 1, 1, 1, 0},

                // BOTTOM

                {1, 0, 1, 0, 0, 0, 1, 0, 0},
                {1, 0, 1, 0, 0, 1, 0, 0, 0},
        }
};
//#endregion

void DrawFrame(HDC deviceCtx) {
    FillRect(deviceCtx, &clientArea, blackBrush);
    SelectObject(deviceCtx, whitePen);

    DrawMesh(dummy_cube, deviceCtx);
}

void MultiplyMatrixVector(Vector3 &i, Vector3 &o, Matrix4x4 &m);

const float zNear = .01f;
const float zFar = 1000;
const int FOV = 90;
const float aspectRatio = wnd_h / wnd_w;

const float fovRad = (1 / tanf(FOV * 0.5f / 180.0f * 3.14159));

Matrix4x4 projectionMatrix = {
        aspectRatio * fovRad, 0, 0, 0,
        0, fovRad, 0, 0,
        0, 0, zFar / (zFar - zNear), 1.0f,
        0, 0, (-zFar * zNear) / (zFar - zNear), 0};

void DrawTriangle3DSpace(Triangle triangle, Vector3 pos, HDC deviceCtx) {
    Triangle projected;

    //Translation and scaling
    for (int i = 0; i < 3; ++i) {
        triangle.vertices[i] *= scale;
        triangle.vertices[i] += pos;
    }

    //calculate normal and normalize it
    Vector3 normal, startLine, endLine;
    startLine.x = triangle.vertices[1].x - triangle.vertices[0].x;
    startLine.y = triangle.vertices[1].y - triangle.vertices[0].y;
    startLine.z = triangle.vertices[1].z - triangle.vertices[0].z;

    endLine.x = triangle.vertices[2].x - triangle.vertices[0].x;
    endLine.y = triangle.vertices[2].y - triangle.vertices[0].y;
    endLine.z = triangle.vertices[2].z - triangle.vertices[0].z;

    normal.x = startLine.y * endLine.z - startLine.z * endLine.y;
    normal.x = startLine.z * endLine.x - startLine.x * endLine.z;
    normal.x = startLine.x * endLine.y - startLine.y * endLine.x;

    float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

    normal /= length;

    //Project to 3D if visible (normal relative to camera is negative)
    if (normal.x * (triangle.vertices[0].x - camPos.x) +
        normal.y * (triangle.vertices[0].y - camPos.y) +
        normal.z * (triangle.vertices[0].z - camPos.z) < 0) {
        for (int i = 0; i < 3; ++i) {
            MultiplyMatrixVector(triangle.vertices[i], projected.vertices[i], projectionMatrix);
        }
    }

    //scale into view
    for (int i = 0; i < 3; ++i) {
        projected.vertices[i] += 1;
        projected.vertices[i].x *= 0.5f * wnd_w;
        projected.vertices[i].y *= 0.5f * wnd_h;
    }

    //Draw the result
    MoveToEx(deviceCtx, projected.vertices[0].x, clientArea.bottom - projected.vertices[0].y, nullptr);
    LineTo(deviceCtx, projected.vertices[1].x, clientArea.bottom - projected.vertices[1].y);
    MoveToEx(deviceCtx, projected.vertices[1].x, clientArea.bottom - projected.vertices[1].y, nullptr);
    LineTo(deviceCtx, projected.vertices[2].x, clientArea.bottom - projected.vertices[2].y);
    MoveToEx(deviceCtx, projected.vertices[2].x, clientArea.bottom - projected.vertices[2].y, nullptr);
    LineTo(deviceCtx, projected.vertices[0].x, clientArea.bottom - projected.vertices[0].y);
}

void DrawMesh(Mesh mesh, HDC deviceCtx) {
    for (auto triangle: mesh.tris) {
        DrawTriangle3DSpace(triangle, position, deviceCtx);
    }
}

void EngineTick() {

}

void MultiplyMatrixVector(Vector3 &i, Vector3 &o, Matrix4x4 &m) {
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0) {
        o.x /= w;
        o.y /= w;
        o.z /= w;
    }
}
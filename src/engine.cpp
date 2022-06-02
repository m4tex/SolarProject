//
// Created by MatexPL on 2022-06-01.
//

#include "windows.h"
#include "vector"
#include "string"
#include "chrono"

namespace Engine {
    //#region Engine datatypes

    struct Vector3 {
        float x, y, z;
    };

    struct Triangle {
        Vector3 vertex[3];
    };

    struct Mesh {
        std::vector<Triangle> tris;
    };

    struct Matrix4 {
        float m[4][4] = {0};
    };

    //#endregion

    //#region variables
    HWND hwnd;
    const Mesh cubeMesh;
    bool drawLoop = true;
    //For FPS calculation
    auto drawTimestamp = std::chrono::high_resolution_clock::now();

    //#endregion

    //#region cube declaration
    const Mesh dummy_cube = {
            .tris = {
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

    void StartDrawLoop();

    void Start(HWND pHwnd) {
        hwnd = pHwnd;
        StartDrawLoop();
        SetWindowTextA(hwnd, "WOOOOOOOOOOOO");
    }

    void StartDrawLoop() {
        PAINTSTRUCT ps;
        HDC hDc = BeginPaint(hwnd, &ps);
        Rectangle(hDc, 10, 10, 100, 100);
        SetWindowTextA(hwnd, "WOOOOOOOOOOOO");
        while (drawLoop) {
//            auto drawElapsed = drawTimestamp - std::chrono::high_resolution_clock::now();
//            char charBuf[32];
//            SetWindowTextA(hwnd, lltoa(drawElapsed.count(), reinterpret_cast<char *>(&charBuf), 10));
//            TextOutA(hDc, 100, 100, lltoa(drawElapsed.count(), reinterpret_cast<char *>(&charBuf), 10), 10);
//            SetPixel(hDc, test, test, RGB(test, 0, 0));
//            test++;
            SetPixel(hDc, 100, 100, RGB(255, 255, 255));
        }
    }
}

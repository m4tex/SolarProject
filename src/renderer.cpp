//
// Created by MatexPL on 2022-06-01.
//
#include "windows.h"
#include "vector"

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

    //#region cube definition
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

    class Renderer {
    public:
        Renderer(HWND hwnd) {
            PAINTSTRUCT ps;
            _hdc = BeginPaint(hwnd, &ps);
            StartDrawLoop();
        };

    private:
        HDC _hdc;
        Mesh cubeMesh;
        bool drawLoop = true;
        int loopTimeSnapshot;

        void StartDrawLoop() {
            while(drawLoop) {

            }
        }
    };
}

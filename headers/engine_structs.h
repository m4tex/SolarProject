//
// Created by MatexPL on 2022-06-06.
//

#ifndef SOLARPROJECT_ENGINE_STRUCTS_H
#define SOLARPROJECT_ENGINE_STRUCTS_H
#include <windows.h>
#include <vector>
#include <string>

namespace Engine {
    struct Vector3 {
        float x,y,z;
        Vector3 operator*(float amount);
        Vector3 operator*(Vector3 vec);
        Vector3 operator/(float amount);
        void operator+=(float amount);
        void operator+=(Vector3 vec);
        void operator*=(Vector3 vec);
        void operator/=(float amount);
    };

    struct Triangle {
        Vector3 vertices[3];
        Vector3 GetMiddlePoint();
        COLORREF color;
    };

    //Maybe make into a class?
    struct Mesh {
        std::vector<Triangle> tris;
        Vector3 origin;
        bool LoadFromFile(std::string path);
    };

    struct Matrix4x4 {
        float m[4][4];
    };
}
#endif //SOLARPROJECT_ENGINE_STRUCTS_H
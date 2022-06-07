//
// Created by MatexPL on 2022-06-07.
//

#ifndef SOLARPROJECT_SOLAROBJECT_H
#define SOLARPROJECT_SOLAROBJECT_H
#include "../headers/engine_structs.h"

namespace Engine {
    class SolarObject {
    public:
        SolarObject(std::string objName);
        std::string name;
        Vector3 position, rotation, scale;

        Mesh mesh;

        float mass;
        Vector3 velocity, angularVelocity;

        Matrix4x4 GetRotationMatrix();
    };
} // Engine

#endif //SOLARPROJECT_SOLAROBJECT_H

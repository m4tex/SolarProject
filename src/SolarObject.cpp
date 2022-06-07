//
// Created by MatexPL on 2022-06-07.
//

#include "../headers/SolarObject.h"
#include <cmath>

namespace Engine {
    SolarObject::SolarObject(std::string objName): name(objName) {
        position = { 0, 0, 0 };
        rotation = { 0, 0, 0 };
        scale = { 1, 1, 1 };
        velocity = { 0, 0, 0 };
        angularVelocity = { 0, 0, 0 };
        mass = 1;
    }

    Matrix4x4 SolarObject::GetRotationMatrix() {

        float xcos = cosf(rotation.x);
        float xsin = sinf(rotation.x);
        float ycos = cosf(rotation.y);
        float ysin = sinf(rotation.y);
        float zcos = cosf(rotation.z);
        float zsin = sinf(rotation.z);

        return {
            zcos*ycos, zcos*ysin*xsin-zsin*xcos, zcos*ysin*xcos+zsin*xsin, 0,
            zsin*ycos, zsin*ysin*xsin+zcos*xcos, zsin*ysin*xcos-zcos*xsin, 0,
              -ysin,           ycos*xsin,                ycos*xcos,        0,
                0,                 0,                        0,            0
        };


//        return {
//            ycos*zcos, xsin*ysin*zcos-xcos*zsin, xcos*ysin*zcos+xsin*zsin, 0,
//            ycos*zcos, xsin*ysin*zsin+xcos*zcos, xcos*ysin*zsin-xsin*zcos, 0,
//            -ysin,             xsin*ycos,                xcos*ycos,        0,
//            0,                     0,                      0,              0
//        };
    }
} // Engine
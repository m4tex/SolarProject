//
// Created by MatexPL on 2022-06-07.
//

#include "../headers/SolarObject.h"

namespace Engine {
    SolarObject::SolarObject(std::string objName): name(objName) {
        position = { 0, 0, 0 };
        rotation = { 0, 0, 0 };
        scale = { 1, 1, 1 };
        velocity = { 0, 0, 0 };
        angularVelocity = { 0, 0, 0 };
        mass = 1;
    }
} // Engine
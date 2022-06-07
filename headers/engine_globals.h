//
// Created by MatexPL on 2022-06-07.
//

#ifndef SOLARPROJECT_ENGINE_GLOBALS_H
#define SOLARPROJECT_ENGINE_GLOBALS_H
#include "../headers/engine_structs.h"
#define wnd_h 500
#define wnd_w 500

namespace Engine {
    const float zNear = .01f;
    const float zFar = 1000;
    const int FOV = 90;
    const float aspectRatio = wnd_h / wnd_w;
}


#endif //SOLARPROJECT_ENGINE_GLOBALS_H

//
// Created by MatexPL on 2022-06-06.
//

#ifndef SOLARPROJECT_ENGINE_METHODS_H
#define SOLARPROJECT_ENGINE_METHODS_H
#include "engine_structs.h"
#include "SolarObject.h"

namespace Engine {
    void AddToRenderQueue(SolarObject obj);
    void Render(HDC deviceCtx);
    int FPS;
//    void DrawMesh(Mesh mesh, HDC deviceCtx);
//    void MultiplyMatrixVector(Vector3 &i, Vector3 &o, Matrix4x4 &m);
    //It exists.. but do we need it outside of the engine's context? Naaaah. Not now at least.
    //void ProjectTriangleToQueue(Triangle triangle, Vector3 pos, std::vector<Triangle> &queue);
}
#endif //SOLARPROJECT_ENGINE_METHODS_H

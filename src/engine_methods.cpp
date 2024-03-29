//
// Created by MatexPL on 2022-06-07.
//
#include <cmath>
#include "../headers/engine_methods.h"
#include "../headers/engine_globals.h"
#include "../headers/SolarObject.h"

//temporary
#define cameraPosition { 0, 0, 0 }
#define baseIllumination 96

using namespace Engine;

const float fovRad = (1 / tanf(FOV * 0.5f / 180.0f * 3.14159));

Matrix4x4 projectionMatrix = {
        aspectRatio * fovRad, 0, 0, 0,
        0, fovRad, 0, 0,
        0, 0, zFar / (zFar - zNear), 1.0f,
        0, 0, (-zFar * zNear) / (zFar - zNear), 0};

std::vector<SolarObject> renderQueue;
std::vector<Triangle> triangleQueue;

void MultiplyMatrixVector(Vector3 &i, Vector3 &o, Matrix4x4 &m) {
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = (i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3]);
    w += (w == 0);

    o.x /= w;
    o.y /= w;
    o.z /= w;
}

Matrix4x4 rotationMatrix;

void ProjectTriangleToQueue(Triangle triangle, SolarObject parent, Vector3 camPos, std::vector<Triangle> &queue) {
    Triangle rotated;
    Triangle projected;

    //Translation, rotation and scaling
    for (int i = 0; i < 3; ++i) {
        MultiplyMatrixVector(triangle.vertices[i], rotated.vertices[i], rotationMatrix);
        rotated.vertices[i] *= parent.scale;
        rotated.vertices[i] += parent.position;
    }

    //calculate normal and normalize it
    Vector3 normal, startLine, endLine;
    startLine.x = rotated.vertices[1].x - rotated.vertices[0].x;
    startLine.y = rotated.vertices[1].y - rotated.vertices[0].y;
    startLine.z = rotated.vertices[1].z - rotated.vertices[0].z;

    endLine.x = rotated.vertices[2].x - rotated.vertices[0].x;
    endLine.y = rotated.vertices[2].y - rotated.vertices[0].y;
    endLine.z = rotated.vertices[2].z - rotated.vertices[0].z;

    normal.x = startLine.y * endLine.z - startLine.z * endLine.y;
    normal.y = startLine.z * endLine.x - startLine.x * endLine.z;
    normal.z = startLine.x * endLine.y - startLine.y * endLine.x;

    float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

    normal /= length;

    //Don't project or draw if the relative normals are positive.
    if (normal.x * (rotated.vertices[0].x - camPos.x) +
        normal.y * (rotated.vertices[0].y - camPos.y) +
        normal.z * (rotated.vertices[0].z - camPos.z) > 0) {
        return;
    }

    //Illumination
    Vector3 lightColor = {
            255 - baseIllumination,
            255 - baseIllumination,
            255 - baseIllumination
    };
    Vector3 lightDirection = {0, 0, -1};
    float l = sqrtf(lightDirection.x * lightDirection.x +
                    lightDirection.y * lightDirection.y +
                    lightDirection.z * lightDirection.z);
    lightDirection /= l;

    //The dot product of the light direction and lighting (similarity)
    float dp = normal.x * lightDirection.x + normal.y * lightDirection.y + normal.z * lightDirection.z;

    projected.color = RGB((int) (lightColor.x * dp + baseIllumination),
                          (int) (lightColor.y * dp + baseIllumination),
                          (int) (lightColor.z * dp + baseIllumination));

    for (int i = 0; i < 3; ++i) {
        MultiplyMatrixVector(rotated.vertices[i], projected.vertices[i], projectionMatrix);
    }

    //Final transformation
    for (int i = 0; i < 3; ++i) {
        //Scale into view
        projected.vertices[i] += 1;
        projected.vertices[i].x *= 0.5f * wnd_w;
        projected.vertices[i].y *= 0.5f * wnd_h;
    }

    queue.push_back(projected);
}

void DrawObject(SolarObject obj, HDC deviceCtx) {
    rotationMatrix = obj.GetRotationMatrix();

    for (auto triangle: obj.mesh.tris) {
        ProjectTriangleToQueue(triangle, obj, cameraPosition, triangleQueue);
    }

    std::sort(triangleQueue.begin(), triangleQueue.end(), [](Triangle &t1, Triangle &t2) {
        float z1 = (t1.vertices[0].z + t1.vertices[1].z + t1.vertices[2].z) / 3;
        float z2 = (t2.vertices[0].z + t2.vertices[1].z + t2.vertices[2].z) / 3;
        return z1 > z2;
    });

    for (auto triangle: triangleQueue) {
        POINT vertex[3];

        for (int i = 0; i < 3; ++i) {
            vertex[i].x = triangle.vertices[i].x;
            vertex[i].y = wnd_h - triangle.vertices[i].y;
        }

        //Draw the result
        HBRUSH brush = CreateSolidBrush(triangle.color);
        HPEN pen = CreatePen(0, 1, triangle.color);

        SelectObject(deviceCtx, brush);
        SelectObject(deviceCtx, pen);
        MoveToEx(deviceCtx, triangle.vertices[0].x, wnd_h - triangle.vertices[0].y, nullptr);
        Polygon(deviceCtx, vertex, 3);
        DeleteObject(brush);
        DeleteObject(pen);

        //wireframe

//        HPEN blackPen = CreatePen(0, 1, RGB(0, 0, 0));
//        SelectObject(deviceCtx, blackPen);
//
//        MoveToEx(deviceCtx, triangle.vertices[0].x, wnd_h - triangle.vertices[0].y, nullptr);
//        LineTo(deviceCtx, triangle.vertices[1].x, wnd_h - triangle.vertices[1].y);
//        MoveToEx(deviceCtx, triangle.vertices[1].x, wnd_h - triangle.vertices[1].y, nullptr);
//        LineTo(deviceCtx, triangle.vertices[2].x, wnd_h - triangle.vertices[2].y);
//        MoveToEx(deviceCtx, triangle.vertices[2].x, wnd_h - triangle.vertices[2].y, nullptr);
//        LineTo(deviceCtx, triangle.vertices[0].x, wnd_h - triangle.vertices[0].y);
    }

    //Empty the triangle queue
    triangleQueue.clear();
}

//Settings
bool rotating = false;ö
bool moving = false;

void UpdateSettings();
void DrawSettings(HDC deviceCtx);

namespace Engine {
    void AddToRenderQueue(SolarObject obj) {
        renderQueue.push_back(obj);
    }

    void Render(HDC deviceCtx) {
        for (int i = 0; i < renderQueue.size(); ++i) {
            DrawObject(renderQueue[i], deviceCtx);
        }
    }
}
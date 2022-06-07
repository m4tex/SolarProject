//
// Created by MatexPL on 2022-06-06.
//
#include "../headers/engine_structs.h"
#include <string>
#include <fstream>
#include <strstream>

using namespace Engine;

//#region Vector3

Vector3 Vector3::operator*(float amount) {
    return {x * amount, y * amount, z * amount};
}

Vector3 Vector3::operator*(Vector3 vec) {
    return {x * vec.x, y * vec.y, z * vec.z};
}

Vector3 Vector3::operator/(float amount) {
    return {x /= amount, y /= amount, z /= amount};
}

void Vector3::operator+=(Vector3 vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
}

void Vector3::operator+=(float amount) {
    x += amount;
    y += amount;
    z += amount;
}

void Vector3::operator*=(Vector3 vec) {
    x *= vec.x;
    y *= vec.y;
    z *= vec.z;
}

void Vector3::operator/=(float amount) {
    x /= amount;
    y /= amount;
    z /= amount;
}

//#endregion

//#region Triangle

Vector3 Triangle::GetMiddlePoint() {
    return {(vertices[1].x - vertices[0].x - 2 * vertices[2].x) / 4,
            (vertices[1].y - vertices[0].y - 2 * vertices[2].y) / 4};
}

//#endregion

//Matrix4x4 doesn't need anything specific (yet)

//#region Mesh


bool Mesh::LoadFromFile(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return 0;
    }

    //Vertex cache
    std::vector<Vector3> vertices;

    while (!file.eof()) {
        char line[128];
        file.getline(line, 128);

        std::strstream stream;
        stream << line;

        //Here I store the line beginnings so that I can access the actual data
        char junk;

        if (line[0] == 'v') {
            Vector3 v;
            stream >> junk >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }

        if (line[0] == 'f') {
            int vertIndexes[3];
            stream >> junk >> vertIndexes[0] >> vertIndexes[1] >> vertIndexes[2];
            tris.push_back({vertices[vertIndexes[0] - 1], vertices[vertIndexes[1] - 1], vertices[vertIndexes[2] - 1]});
        }
    }

    return 1;
}

//#endregion

//#region SolarObject
//#endregion
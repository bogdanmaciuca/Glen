#pragma once
#include <vector>
#include "int.h"
#include "mathdef.h"

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
    Vec3 bitangent;
};

using Index = u16;

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Index>  indices;
    void*               texture;
    u32                 texture_byte_size;
};


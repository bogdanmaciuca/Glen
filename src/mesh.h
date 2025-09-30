#pragma once
#include <vector>
#include <array>
#include "int.h"
#include "mathdef.h"

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
    Vec3 tangent;
    Vec3 bitangent;
};

using Index = u16;

enum {
    TEX_DIFFUSE = 0,
    TEX_NORMAL,
    TEX_SPECULAR,

    TEX_COUNT
};

enum {
    TEX_DIFFUSE_BIT = 1 << TEX_DIFFUSE,
    TEX_NORMAL_BIT = 1 << TEX_NORMAL,
};

struct TextureData {
    void*  pixels = nullptr;
    i32    width  = 0;
    i32    height = 0;
    ~TextureData();
    size_t ByteSize();
};

using TextureArray = std::array<TextureData, TEX_COUNT>;

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Index>  indices;
    TextureArray        textures;
};


#pragma once
#include <array>
#include "int.h"
#include "mathdef.h"
#include "mesh.h"
#include "renderer_shader.h"

class Object {
public:
    Object()              = default;
    Object(const Object&) = delete;
    Object(Object&&)      = delete;
    void Initialize();
    void UpdateGeometry(const Mesh& mesh);
    static void SetAttributes();
    void Draw(Shader& shader);
private:
    u32 m_vbo = 0;
    u32 m_ebo = 0;
    u32 m_indices_count = 0;
    std::array<u32, TEX_COUNT> m_textures;
    u32 m_available_textures = 0;

    Mat4 m_model = Mat4(1.0f);
};


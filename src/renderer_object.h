#pragma once
#include "int.h"
#include "mesh.h"

class Object {
public:
    Object()              = default;
    Object(const Object&) = delete;
    Object(Object&&)      = delete;
    void Initialize();
    void UpdateGeometry(const Mesh& mesh);
    static void SetAttributes();
    void Draw();
private:
    u32 m_vbo = 0;
    u32 m_ebo = 0;
    u32 m_indices_count = 0;
};


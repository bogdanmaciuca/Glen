#pragma once
#include "int.h"
#include "mathdef.h"

constexpr float DEFAULT_FOV    = 77.0f;
constexpr float DEFAULT_Z_NEAR = 0.01f;
constexpr float DEFAULT_Z_FAR  = 100.0f;

namespace renderer {
    bool Initialize();
    void Release();
    void RenderFrame();
    bool EnsureResized(i32 width, i32 height);

    u32 GetOutputTexture();
    void SetCamera(const Vec3& cam_pos, const Mat4& view_matrix);
}


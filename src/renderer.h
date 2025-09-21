#pragma once
#include "int.h"
#include "mathdef.h"

namespace renderer {
    bool Initialize();
    void Release();
    void RenderFrame();
    void Resize(u32 new_width, u32 new_height);

    u32 GetOutputTexture();
    void SetViewMatrix(const Mat4& view_matrix);
}


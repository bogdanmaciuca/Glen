#pragma once
#include <string>
#include "int.h"
#include "mathdef.h"

class Shader {
public:
    Shader() = default;
    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    ~Shader();
    void Initialize(const std::string& vert_source, const std::string& frag_source);
    void Use();
    void SetInt(const std::string& name,int value);
    void SetVec3(const std::string& name, const Vec3& value);
    void SetMat4(const std::string& name, const Mat4& value);
private:
    u32 m_id = 0;
};


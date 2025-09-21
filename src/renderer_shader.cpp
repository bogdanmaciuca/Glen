#include "renderer_shader.h"
#include <string>
#include "SDL3/SDL_opengles2.h"
#include "log.h"

GLuint CreateShaderStage(GLenum stage, const std::string& source);
void CheckErrors(GLuint shader, GLenum stage);

Shader::~Shader() {
    if (m_id != 0)
        glDeleteShader(m_id);
}

// TODO: make this return a bool for whether the shader compiled or not
void Shader::Initialize(const std::string& vert_source, const std::string& frag_source) {
    GLuint vert_shader = CreateShaderStage(GL_VERTEX_SHADER, vert_source);
    GLuint frag_shader = CreateShaderStage(GL_FRAGMENT_SHADER, frag_source);
    m_id = glCreateProgram();
    glAttachShader(m_id, vert_shader);
    glAttachShader(m_id, frag_shader);
    glLinkProgram(m_id);

    glDetachShader(m_id, vert_shader);
    glDeleteShader(vert_shader);
    glDetachShader(m_id, frag_shader);
    glDeleteShader(frag_shader);
}

void Shader::Use() {
    glUseProgram(m_id);
}

// TODO: Maybe cache these somehow
void Shader::SetInt(const std::string& name,int value) {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); 
}

void Shader::SetVec3(const std::string& name, const Vec3& value) {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]); 
}

void Shader::SetMat4(const std::string& name,const Mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &value[0][0]); 
}

GLuint CreateShaderStage(GLenum stage, const std::string& source) {
    GLuint shader = glCreateShader(stage);
    const char* source_c_str = source.c_str();
    glShaderSource(shader, 1, &source_c_str, nullptr);
    glCompileShader(shader);
    CheckErrors(shader, stage);
    return shader;
}

void CheckErrors(GLuint shader, GLenum stage) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        char error_msg[2048];
        glGetShaderInfoLog(shader, 2047, nullptr, error_msg);
        std::string type_str;
        switch(stage) {
            case GL_VERTEX_SHADER: type_str = "GL_VERTEX_SHADER"; break;
            case GL_FRAGMENT_SHADER: type_str = "GL_FRAGMENT_SHADER"; break;
        }
        Log("Error in {} shader {}:\n{}", type_str.c_str(), shader, error_msg);
    }
}


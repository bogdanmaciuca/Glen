// TODO:
// - get window size from editor

#include "renderer.h"
#include "SDL3/SDL_opengles2.h"
#include "glm/gtc/matrix_transform.hpp"
#include "resource.h"
#include "mesh.h"
#include "mathdef.h"
#include "renderer_shader.h"
#include "renderer_object.h"

namespace renderer {
    namespace {
        GLuint m_output_framebuffer;
        GLuint m_output_texture;
        GLuint m_output_renderbuffer;

        Mat4 m_proj_matrix;
        Mat4 m_view_matrix;

        Object m_obj;
        Shader m_shader;
    }

    bool CreateFramebuffer();
    void DestroyFramebuffer();

    bool Initialize() {
        if (CreateFramebuffer() == false)
            return false;

        m_shader.Initialize(
            ReadFileAsString("shaders/vertex.glsl"),
            ReadFileAsString("shaders/fragment.glsl")
        );

        Mesh mesh = {
            .vertices = {
                { Vec3( 0.0f,  1.0f, 0.0f) },
                { Vec3(-1.0f, -1.0f, 1.0f) },
                { Vec3( 1.0f, -1.0f, 0.0f) },
            },
            .indices = { 0, 1, 2 }
        };
        m_obj.Initialize();
        m_obj.UpdateGeometry(mesh);

        return true;
    }

    void Release() {
        DestroyFramebuffer();
    }

    void RenderFrame() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_output_framebuffer);

        glClearColor(0.6f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_shader.Use();
        m_shader.SetMat4("u_model", Mat4(1));
        m_shader.SetMat4("u_view", m_view_matrix);
        m_shader.SetMat4(
            "u_proj",
            glm::perspective(glm::radians(80.0f), 1024.0f/768.0f, 0.1f, 100.0f)
        );
        Object::SetAttributes();
        m_obj.Draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Resize(u32 new_width, u32 new_height) {

    }

    u32 GetOutputTexture() {
        return m_output_texture;
    }

    void SetViewMatrix(const Mat4& view_matrix) {
        m_view_matrix = view_matrix;
    }

    bool CreateFramebuffer() {
        glGenFramebuffers(1, &m_output_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_output_framebuffer);

        glGenTextures(1, &m_output_texture);
        glBindTexture(GL_TEXTURE_2D, m_output_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_output_texture, 0);

        glGenRenderbuffers(1, &m_output_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_output_renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1024, 768);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_output_renderbuffer);

        bool success = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return success;
    }

    void DestroyFramebuffer() {
        glDeleteTextures(1, &m_output_texture);
        glDeleteRenderbuffers(1, &m_output_renderbuffer);
        glDeleteFramebuffers(1, &m_output_framebuffer);
    }
}



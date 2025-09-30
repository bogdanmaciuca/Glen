// TODO:
// - get window size from editor

#include "renderer.h"
#include "SDL3/SDL_opengles2.h"
#include "glm/gtc/matrix_transform.hpp"
#include "log.h"
#include "resource.h"
#include "mesh.h"
#include "mathdef.h"
#include "renderer_shader.h"
#include "renderer_object.h"
#include "platform.h"

namespace renderer {
    namespace {
        i32    m_width               = 0;
        i32    m_height              = 0;
        GLuint m_output_framebuffer  = 0;
        GLuint m_output_texture      = 0;
        GLuint m_output_renderbuffer = 0;

        float m_FOV    = DEFAULT_FOV;
        float m_z_near = DEFAULT_Z_NEAR;
        float m_z_far  = DEFAULT_Z_FAR;
        Mat4  m_proj_matrix;
        Mat4  m_view_matrix;
        Vec3  m_cam_pos;

        Object m_obj;
        Shader m_shader;
    }

    bool CreateFramebuffer();
    void DestroyFramebuffer();

    bool Initialize() {
        m_shader.Initialize(
            ReadFileAsString("shaders/vertex.glsl"),
            ReadFileAsString("shaders/fragment.glsl")
        );
        m_shader.Use();
        m_shader.SetInt("u_diffuse", TEX_DIFFUSE);
        m_shader.SetInt("u_normal", TEX_NORMAL);
        m_shader.SetInt("u_use_diffuse", 0);
        m_shader.SetInt("u_use_normal", 0);
        m_shader.SetVec3("u_color", Vec3(0.5f, 0.4f, 0.8f));

        Mesh mesh;
        //assert(LoadMesh("tests/res/suzanne/suzanne.obj", &mesh));
        //assert(LoadMeshOBJ("tests/res/stone/stone.obj", &mesh));
        assert(LoadMeshNFG("tests/res/water/water.nfg", nullptr, &mesh));
        m_obj.Initialize();
        m_obj.UpdateGeometry(mesh);

        m_proj_matrix = glm::perspective(
            glm::radians(m_FOV), (float)DEFAULT_WIN_WIDTH / (float)DEFAULT_WIN_HEIGHT, m_z_near, m_z_far);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);  

        return true;
    }

    void Release() {
        DestroyFramebuffer();
    }

    void RenderFrame() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_output_framebuffer);

        glClearColor(0.6f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_shader.Use();
        m_shader.SetMat4("u_model", Mat4(1));
        m_shader.SetMat4("u_view", m_view_matrix);
        m_shader.SetVec3("u_cam_pos", m_cam_pos);
        Object::SetAttributes();
        m_obj.Draw();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    bool EnsureResized(i32 width, i32 height) {
        if (m_width == width && m_height == height)
            return true;
        
        m_width = width;
        m_height = height;
        glViewport(0, 0, m_width, m_height);
        DestroyFramebuffer();
        CreateFramebuffer();
        m_proj_matrix = glm::perspective(
            glm::radians(m_FOV), (float)m_width / m_height, m_z_near, m_z_far);
        m_shader.SetMat4("u_proj", m_proj_matrix);

        return false;
    }

    u32 GetOutputTexture() {
        return m_output_texture;
    }

    void SetCamera(const Vec3& cam_pos, const Mat4& view_matrix) {
        m_cam_pos = cam_pos;
        m_view_matrix = view_matrix;
    }

    bool CreateFramebuffer() {
        glGenFramebuffers(1, &m_output_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_output_framebuffer);

        glGenTextures(1, &m_output_texture);
        glBindTexture(GL_TEXTURE_2D, m_output_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_output_texture, 0);

        glGenRenderbuffers(1, &m_output_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_output_renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_output_renderbuffer);

        bool success = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return success;
    }

    void DestroyFramebuffer() {
        if (m_output_framebuffer == 0) return;

        glDeleteTextures(1, &m_output_texture);
        glDeleteRenderbuffers(1, &m_output_renderbuffer);
        glDeleteFramebuffers(1, &m_output_framebuffer);
    }
}



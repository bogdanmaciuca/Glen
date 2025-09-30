#include "renderer_object.h"
#include "SDL3/SDL_opengles2.h"
#include "log.h"
#include "renderer_shader.h"

void Object::Initialize() {
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenTextures(TEX_COUNT, &m_textures[0]);
}

void Object::UpdateGeometry(const Mesh& mesh) {
    m_indices_count = mesh.indices.size();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        mesh.vertices.size() * sizeof(*mesh.vertices.begin()),
        mesh.vertices.data(),
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh.indices.size() * sizeof(*mesh.indices.begin()),
        mesh.indices.data(),
        GL_STATIC_DRAW
    );

    const auto InitTexture = [&mesh, this](u32 tex_idx, const TextureData& tex_data) {
        glBindTexture(GL_TEXTURE_2D, m_textures[tex_idx]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA,
            mesh.textures[tex_idx].width,
            mesh.textures[tex_idx].height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, mesh.textures[tex_idx].pixels
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    };
    
    for (i32 i = 0; i < TEX_COUNT; i++) {
        if (mesh.textures[i].pixels != nullptr) {
            InitTexture(i, mesh.textures[i]);
            m_available_textures |= (1 << i);
        }
    }
}

void Object::Draw() {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    for (u32 i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    }
    glDrawElements(GL_TRIANGLES, m_indices_count, GL_UNSIGNED_SHORT, nullptr);
}

void Object::SetAttributes() {
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    // Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
}


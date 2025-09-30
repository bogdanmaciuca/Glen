// TODO:
// - GetPath()
// - use GetPath() instead of std::filesystem
// - 
#include "resource.h"
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "../vendor/stb/stb_image.h"
#include "log.h"

// TODO: maybe use fstream for this?
std::vector<u8> ReadFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    assert(file != nullptr);

    fseek(file, 0, SEEK_END);
    u32 len = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::vector<u8> result(len + 1);
    fread(result.data(), sizeof(u8), len + 1, file);

    fclose(file);
    return result;
}

std::string ReadFileAsString(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    assert(file != nullptr);

    fseek(file, 0, SEEK_END);
    u32 len = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::string result(len + 1, 0);
    fread(result.data(), sizeof(u8), len + 1, file);

    fclose(file);
    return result;
}

struct ObjIndex {
    u32 pos_idx;
    u32 normal_idx;
    u32 uv_idx;
    bool operator==(const ObjIndex& other) const {
        return pos_idx == other.pos_idx &&
        normal_idx == other.normal_idx &&
        uv_idx == other.uv_idx;
    }
};


template <>
struct std::hash<ObjIndex> {
    std::size_t operator()(const ObjIndex& k) const {
        return ((hash<u32>()(k.pos_idx)
        ^ (hash<u32>()(k.normal_idx) << 1)) >> 1)
        ^ (hash<u32>()(k.uv_idx) << 1);
    }
};

std::string GetRelativePath(std::string filepath) {
    std::filesystem::path p(filepath);
    return p.remove_filename().string();
}

bool ParseMtl(const std::string& filepath, std::string* material, TextureArray* textures);
bool LoadMeshOBJ(const char* filepath, Mesh* mesh) {
    // For all faces' vertices, we will either:
    //   - append a vertex to the mesh's vertices vector (if it's not in the
    //   unordered_map i.e. it's unique)
    //   - append the index of the vertex in the mesh's vertices vector to
    //   the mesh's indices vector if it already exists

    bool status = true;

    std::unordered_map<ObjIndex, u32> vertices_map; // TODO: maybe use open addressing hashmap instead
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;

    const auto AddVertex = [&](const ObjIndex& vertex) {
        if (vertices_map.count(vertex) == 0) {
            mesh->vertices.push_back(Vertex{
                positions[vertex.pos_idx],
                normals[vertex.normal_idx],
                uvs[vertex.uv_idx],
            });
            vertices_map[vertex] = mesh->vertices.size() - 1;
            mesh->indices.push_back(mesh->vertices.size() - 1);
        }
        else {
            mesh->indices.push_back(vertices_map[vertex]);
        }
    };

    std::ifstream file(filepath);
    if (!file.is_open())
        return false;

    std::string keyword;
    while (file >> keyword) {
        if (keyword == "v") {
            Vec3 pos;
            file >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (keyword == "vn") {
            Vec3 normal;
            file >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (keyword == "vt") {
            Vec2 uv;
            file >> uv.x >> uv.y;
            uvs.push_back(uv);
        }
        else if (keyword == "f") {
            const auto ParseFaceVertex = [&file]() -> ObjIndex {
                ObjIndex fv;
                std::string str;
                file >> str;
                std::istringstream ss(str);

                std::string token;
                int i = 0;
                while (std::getline(ss, token, '/')) {
                    if (!token.empty()) {
                        int value = std::stoi(token);
                        if      (i == 0) fv.pos_idx    = value - 1;
                        else if (i == 1) fv.uv_idx     = value - 1;
                        else if (i == 2) fv.normal_idx = value - 1;
                    }
                    i++;
                }
                return fv;
            };
            for (i32 i = 0; i < 3; i++)
                AddVertex(ParseFaceVertex());
        }
        else if (keyword == "mtllib") {
            std::string mtl_filepath;
            file >> mtl_filepath;

            if(!ParseMtl(GetRelativePath(filepath) + mtl_filepath, nullptr, &mesh->textures)) {
                Log("Could not load material file.");
                status = false;
            }
        }
        else if (keyword == "usemtl") {
            Log("Encountered usemtl, will ignore as it is not implemented yet");
            // TODO:
        }
    }

    return status;
}

bool ParseMtl(const std::string& filepath, std::string* material, TextureArray* textures) {
    std::ifstream file(filepath);
    if (!file.is_open())
        return false;

    const auto LoadTexture =
        [&filepath, textures](i32 tex_idx, const std::string& texture_path) -> bool {
            (*textures)[tex_idx].pixels = stbi_load(
                (GetRelativePath(filepath) + texture_path).c_str(),
                &(*textures)[tex_idx].width,
                &(*textures)[tex_idx].height,
                nullptr,
                STBI_rgb_alpha
            );
            if ((*textures)[tex_idx].pixels == nullptr) {
                Log("Could not open texture file: {}", texture_path);
                return false;
            }
            return true;
        };

    bool encountered_newmtl = false;
    std::string keyword;
    while (file >> keyword) {
        if (keyword == "newmtl") {
            if (encountered_newmtl == true) {
                Log("Only one material is allowed at the moment. Parsing stopped.");
                return true;
            }
            encountered_newmtl = true;
            std::string material_name;
            file >> material_name;
    
            if (material != nullptr)
                *material = material_name;
        }
        else if (keyword == "map_Kd") {
            std::string texture_path;
            file >> texture_path;
            if (!LoadTexture(TEX_DIFFUSE, texture_path))
                return false;
        }
        else if (keyword == "norm") {
            std::string texture_path;
            file >> texture_path;
            if (!LoadTexture(TEX_NORMAL, texture_path))
                return false;
        }
    }

    return true;
}

// Load: display line on which the error occured with a nice error message
bool LoadMeshNFG(const char* filepath, const char* texture_filepath, Mesh* mesh) {
    FILE* file = fopen(filepath, "r");
    if (file == nullptr)
        return false;

#define SCAN(fmt, narg, ...) \
    { \
    if (fscanf(file, fmt, __VA_ARGS__) != narg) \
        goto fail_cleanup; \
    }

    i32 vertices_num;
    SCAN("NrVertices: %d", 1, &vertices_num);
    for (i32 i = 0; i < vertices_num; i++) {
        i32 idx;
        Vertex vtx;
        SCAN(
            " %d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f]; ", 15,
            &idx,
            &vtx.position.x, &vtx.position.y, &vtx.position.z,
            &vtx.normal.x, &vtx.normal.y, &vtx.normal.z,
            &vtx.bitangent.x, &vtx.bitangent.y, &vtx.bitangent.z,
            &vtx.tangent.x, &vtx.tangent.y, &vtx.tangent.z,
            &vtx.uv.x, &vtx.uv.y
        );
        if (idx != i)
            goto fail_cleanup;
        mesh->vertices.push_back(vtx);
    }

    i32 indices_num;
    SCAN("NrIndices: %d", 1, &indices_num);
    for (i32 i = 0; i < indices_num / 3; i++) {
        i32 idx;
        i32 i1, i2, i3;
        Log("{}", i);
        SCAN(" %d. %d, %d, %d", 4, &idx, &i1, &i2, &i3);
        if (idx != i)
            goto fail_cleanup;
        mesh->indices.push_back(i1);
        mesh->indices.push_back(i2);
        mesh->indices.push_back(i3);
    }

#undef SCAN
    return true;
fail_cleanup:
    fclose(file);
    return false;
}

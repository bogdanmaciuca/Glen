#include "resource.h"
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
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
// TODO: handle errors
//       use std::expected
//       maybe move to obj_parser.h or smth
bool LoadMesh(const char* filepath, Mesh* mesh) {
    // For all faces' vertices, we will either:
    //   - append a vertex to the mesh's vertices vector (if it's not in the
    //   unordered_map i.e. it's unique)
    //   - append the index of the vertex in the mesh's vertices vector to
    //   the mesh's indices vector if it already exists

    std::unordered_map<ObjIndex, u32> vertices_map; // TODO: maybe use open addressing hashmap instead
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;

    const auto AddVertex = [&](const ObjIndex& vertex) {
        if (vertices_map.count(vertex) == 0) {
            //Log("positions.size() = {}, vertex.pos_idx = {}", positions.size(), vertex.pos_idx);
            //Log("normals.size() = {}, vertex.normal_idx = {}", normals.size(), vertex.normal_idx);
            //Log("uvs.size() = {}, vertex.uv_idx = {}", uvs.size(), vertex.uv_idx);
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
            std::string mtl_path;
            file >> mtl_path;
            // TODO: read mtl file (maybe)
        }
        else if (keyword == "usemtl") {
            // TODO:
        }
    }

    return true;
}


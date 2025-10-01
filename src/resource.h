#pragma once
#include <vector>
#include <string>
#include "int.h"
#include "mesh.h"

std::vector<u8> ReadFile(const char* filepath);

std::string ReadFileAsString(const char* filepath);

bool LoadMeshOBJ(const char* filepath, Mesh* mesh);

bool LoadMeshNFG(const char* filepath, const TexturePathArray& texture_paths, Mesh* mesh);


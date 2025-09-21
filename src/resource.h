#pragma once
#include <vector>
#include <string>
#include "int.h"
#include "mesh.h"

std::vector<u8> ReadFile(const char* filepath);

std::string ReadFileAsString(const char* filepath);

bool LoadMesh(const char* filepath, Mesh* mesh);


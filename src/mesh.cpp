#include "mesh.h"
#include "../vendor/stb/stb_image.h"

Texture::~Texture() {
    if (pixels != nullptr)
        stbi_image_free(pixels);
}

size_t Texture::ByteSize() {
    return width * height * 4; // 4 channels at the moment!!!
}


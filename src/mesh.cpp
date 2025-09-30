#include "mesh.h"
#include "../vendor/stb/stb_image.h"

TextureData::~TextureData() {
    if (pixels != nullptr)
        stbi_image_free(pixels);
}

size_t TextureData::ByteSize() {
    return width * height * 4; // 4 channels at the moment!!!
}


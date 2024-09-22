#include "texture.h"
#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <stb/stb_image.h>

unsigned int load_texture(const std::filesystem::path& path) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // Mipmap filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load image, create texture and generate mipmaps
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true); // Flip loaded texture's y-axis
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cout << "Unable to load image: " << stbi_failure_reason() << "\n";
        std::abort();
    }

    GLenum internalFormat;
    GLenum format;

    if (channels == 1) {
        internalFormat = GL_RED;
        format = GL_RED;
    } else if (channels == 3) {
        internalFormat = GL_RGB;
        format = GL_RGB;
    } else if (channels == 4) {
        internalFormat = GL_RGBA;
        format = GL_RGBA;
    }

    // Upload texture to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}

unsigned int load_texture_array(const std::vector<std::filesystem::path>& paths) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Assuming all images have the same dimensions and format
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    // First, we need to know the dimensions of the images
    unsigned char* data = stbi_load(paths[0].c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cout << "Unable to load image: " << stbi_failure_reason() << "\n";
        std::abort();
    }
    stbi_image_free(data);

    GLenum internalFormat, format;
    if (channels == 1) {
        internalFormat = GL_R8;
        format = GL_RED;
    } else if (channels == 3) {
        internalFormat = GL_RGB8;
        format = GL_RGB;
    } else if (channels == 4) {
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
    }

    // Allocate storage for the texture array
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, width, height, paths.size(), 0, format, GL_UNSIGNED_BYTE, nullptr);

    // Load each image into a layer of the texture array
    for (int i = 0; i < paths.size(); ++i) {
        data = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cout << "Unable to load image: " << stbi_failure_reason() << "\n";
            std::abort();
        }
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    return texture;
}

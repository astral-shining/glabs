#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <cpputils/types.hpp>
#include <cpputils/debug.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_int3.hpp>

#include "imagedata.hpp"

namespace GL {

template<u32 target>
class Texture { 
    u32 m_id;

public:
    Texture(u32 option_filter = GL_NEAREST, u32 option_wrap = GL_CLAMP_TO_BORDER);

    auto& setImage(
        i32 internalformat, 
        i32 format, 
        i32 width, 
        void* data, 
        u32 type = target, 
        u32 detail = 0, 
        u32 border = 0
    ) requires (target == GL_TEXTURE_1D);
    auto& setImage(
        i32 internalformat, 
        i32 format, 
        glm::ivec2 size, 
        void* data, 
        u32 type = target, 
        u32 detail = 0, 
        u32 border = 0
    ) requires (target == GL_TEXTURE_2D);
    auto& setImage(
        i32 internalformat, 
        i32 format, 
        glm::ivec3 size, 
        void* data, 
        u32 type = target, 
        u32 detail = 0, 
        u32 border = 0
    ) requires (target == GL_TEXTURE_3D);
    
    auto& subImage(
        i32 format, 
        i32 offsetX, 
        i32 width, 
        void* data, 
        u32 type = target, 
        u32 detail = 0, 
        u32 border = 0
    ) requires (target == GL_TEXTURE_1D);
    auto& subImage(
        i32 format, 
        glm::ivec2 offset, 
        glm::ivec2 size, 
        void* data, 
        u32 type = target, 
        u32 detail = 0, 
        u32 border = 0
    ) requires (target == GL_TEXTURE_2D);
    auto& subImage(
        i32 format, 
        glm::ivec3 offset, 
        glm::ivec3 size, 
        void* data, 
        u32 type = target, 
        u32 detail = 0, 
        u32 border = 0
    ) requires (target == GL_TEXTURE_3D);

    auto& use();
    auto& unuse();
    ~Texture();
};

template<u32 target>
inline Texture<target>::Texture(u32 option_filter, u32 option_wrap) {
    glGenTextures(1, &m_id);
    use();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, option_wrap);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, option_wrap);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, option_filter);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, option_filter);
    logDebug("Created texture %d", m_id);
    unuse();
}

template<u32 target>
inline auto& Texture<target>::setImage(
    i32 internalformat, 
    i32 format, 
    i32 width, 
    void* data, 
    u32 type, 
    u32 detail, 
    u32 border
) requires (target == GL_TEXTURE_1D) {
    glTexImage1D(
        type, 
        detail, 
        internalformat,
        width, 
        border, 
        format, 
        type, 
        data
    );
    return *this;
}

template<u32 target>
inline auto& Texture<target>::setImage(
    i32 internalformat, 
    i32 format, 
    glm::ivec2 size, 
    void* data, 
    u32 type, 
    u32 detail, 
    u32 border
) requires (target == GL_TEXTURE_2D) {
    glTexImage2D(
        type, 
        detail, 
        internalformat, 
        size.x, 
        size.y, 
        border, 
        format, 
        type, 
        data
    ); 
    return *this;
}

template<u32 target>
inline auto& Texture<target>::setImage(
    i32 internalformat, 
    i32 format, 
    glm::ivec3 size, 
    void* data, 
    u32 type, 
    u32 detail, 
    u32 border
) requires (target == GL_TEXTURE_3D) {
    glTexImage3D(
        type, 
        detail, 
        internalformat, 
        size.x, 
        size.y, 
        size.z, 
        border, 
        format, 
        type,
        data
    );
    return *this;
}

template<u32 target>
inline auto& Texture<target>::subImage(
    i32 format, 
    i32 offsetX, 
    i32 width, 
    void* data, 
    u32 type, 
    u32 detail, 
    u32 border
) requires (target == GL_TEXTURE_1D) {
    glTexSubImage1D(
        type, 
        detail, 
        offsetX, 
        width, 
        format, 
        type, 
        data
    );
    return *this;
}

template<u32 target>
inline auto& Texture<target>::subImage(
    i32 format, 
    glm::ivec2 offset, 
    glm::ivec2 size, 
    void* data, 
    u32 type, 
    u32 detail, 
    u32 border
) requires (target == GL_TEXTURE_2D) {
    glTexSubImage2D(
        type, 
        detail, 
        offset.x, 
        offset.y, 
        size.x, 
        size.y, 
        format, 
        type, 
        data
    );
    return *this;
}

template<u32 target>
inline auto& Texture<target>::subImage(
    i32 format, 
    glm::ivec3 offset, 
    glm::ivec3 size, 
    void* data, 
    u32 type, 
    u32 detail, 
    u32 border
) requires (target == GL_TEXTURE_3D) {
    glTexSubImage3D(
        type, 
        detail, 
        offset.x, 
        offset.y, 
        offset.z, 
        size.x, 
        size.y, 
        size.z, 
        format, 
        type, 
        data
    );
    return *this;
}

template<u32 target>
inline auto& Texture<target>::use() {
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(type, m_id);
    return *this;
}

template<u32 target>
inline auto& Texture<target>::unuse() {
    glBindTexture(image, 0);
    return *this;
}

template<u32 target>
inline Texture<target>::~Texture() {
    glDeleteTextures(1, &m_id);
    logDebug("Destroyed texture %d", m_id);
}

};



// inline void Texture::setImage(std::string imagestr) {
//     FILE *fp = fopen(("./res/" + imagestr + ".png").data(), "rb");
//     if (!fp) {
//         throw std::runtime_error(std::string("ERROR: Image not found ") + imagestr);
//     }
//     static auto onErr = [] (png_structp pngPtr, png_const_charp msg) {
//         throw std::runtime_error(std::string("ERROR: ") + msg);
//     };
//     static auto onWarn = [] (png_structp pngPtr, png_const_charp msg) {
//         throw std::runtime_error(std::string("WARN:") + msg);
//     };

//     png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, onErr, onWarn);
//     if (!png) {
//         throw std::runtime_error("ERROR: creating png struct");
//     }
    
//     png_infop info = png_create_info_struct(png);
//     png_init_io(png, fp);
//     png_read_info(png, info);

//     int color_type;
//     int bit_depth;
//     png_get_IHDR( png, info, &m_size.x, &m_size.y, &bit_depth, &color_type, NULL, NULL, NULL );
    
//     u32 row_size = png_get_rowbytes(png, info);
//     png_bytep data = (png_bytep) malloc(m_size.y * row_size * sizeof(png_byte));
//     png_bytepp row_pointers = (png_bytepp) malloc(m_size.y * sizeof(png_bytep));

//     for (u32 i = 0; i < m_size.y; i++) {
//         row_pointers[m_size.y-1-i] = data + (i * row_size);
//     }

//     png_read_image(png, row_pointers);

//     int alpha {};
//     switch (png_get_color_type(png, info)) {
//         case PNG_COLOR_TYPE_RGBA:
//             alpha = GL_RGBA;
//             break;
//         case PNG_COLOR_TYPE_RGB:
//             alpha = GL_RGB;
//             break;
//         default:
//             std::runtime_error("Color type " + std::to_string(png_get_color_type(png, info)) + "not supported!");
//     }

//     fclose(fp);
//     free(row_pointers);
//     png_destroy_read_struct(&png, &info, NULL);

//     use();
//     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexImage2D(GL_TEXTURE_2D, 0, alpha, m_size.x, m_size.y, 0, alpha, GL_UNSIGNED_BYTE, data); 
//     free(data);
// }
#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include <png.h>
#include <qwertyutils/utils.hpp>

namespace GL {

class Texture;
inline Texture* current_texture;

class Texture {
    u32 m_id;
    glm::uvec2 m_size;

public:
    Texture();
    Texture(std::string image);
    //Texture(u32 texture);
    inline Texture& operator=(Texture& other);
    Texture& operator=(std::string name);
    void create();
    void setImage(std::string imagestr);
    void destroy();
    void use();
    void unuse();
    u32 getId();
    ~Texture();
};

inline Texture::Texture() {
    create();
}

inline Texture::Texture(std::string image) {
    create();
    setImage(image);
}

inline Texture& Texture::operator=(Texture& other) {
    m_size = other.m_size;
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

inline Texture& Texture::operator=(std::string name) {
    destroy();
    create();
    setImage(name);
    return *this;
}

inline void Texture::create() {
    glGenTextures(1, &m_id);
    logDebug("Created texture %d", m_id);
}

inline void Texture::setImage(std::string imagestr) {
    FILE *fp = fopen(("./res/" + imagestr + ".png").data(), "rb");
    if (!fp) {
        throw std::runtime_error(std::string("ERROR: Image not found ") + imagestr);
    }
    static auto onErr = [] (png_structp pngPtr, png_const_charp msg) {
        throw std::runtime_error(std::string("ERROR: ") + msg);
    };
    static auto onWarn = [] (png_structp pngPtr, png_const_charp msg) {
        throw std::runtime_error(std::string("WARN:") + msg);
    };

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, onErr, onWarn);
    if (!png) {
        throw std::runtime_error("ERROR: creating png struct");
    }
    
    png_infop info = png_create_info_struct(png);
    png_init_io(png, fp);
    png_read_info(png, info);

    int color_type;
    int bit_depth;
    png_get_IHDR( png, info, &m_size.x, &m_size.y, &bit_depth, &color_type, NULL, NULL, NULL );
    
    u32 row_size = png_get_rowbytes(png, info);
    png_bytep data = (png_bytep) malloc(m_size.y * row_size * sizeof(png_byte));
    png_bytepp row_pointers = (png_bytepp) malloc(m_size.y * sizeof(png_bytep));

    for (u32 i = 0; i < m_size.y; i++) {
        row_pointers[m_size.y-1-i] = data + (i * row_size);
    }

    png_read_image(png, row_pointers);

    int alpha {};
    switch (png_get_color_type(png, info)) {
        case PNG_COLOR_TYPE_RGBA:
            alpha = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_RGB:
            alpha = GL_RGB;
            break;
        default:
            std::runtime_error("Color type " + std::to_string(png_get_color_type(png, info)) + "not supported!");
    }

    fclose(fp);
    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);

    use();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, alpha, m_size.x, m_size.y, 0, alpha, GL_UNSIGNED_BYTE, data); 
    free(data);
}

inline void Texture::destroy() {
    glDeleteTextures(1, &m_id);
    logDebug("Destroyed texture %d", m_id);
}

inline void Texture::use() {
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_id);
    current_texture = this;
}

inline void Texture::unuse() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

inline u32 Texture::getId() {
    return m_id;
}

inline Texture::~Texture() {
    destroy();
}

};

#pragma once
#include <cstdint>
#include <initializer_list>
#include <glad/glad.h>
#include <cpputils/types.hpp>

namespace GL {

struct EBO {
    u32 vbo;
    
    inline EBO() {
        glGenBuffers(1, &vbo);
    }
    
    inline void bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    }
    
    inline void unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , 0);
    }

    inline ~EBO() {
        glDeleteBuffers(1, &vbo);
    }

    inline void bufferDataStatic(std::initializer_list<u32> l) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, l.size() * sizeof(u32), l.begin(), GL_STATIC_DRAW);
    }

    inline void bufferDataDynamic(std::initializer_list<u32> l) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, l.size() * sizeof(u32), l.begin(), GL_DYNAMIC_DRAW);
    }
};

};

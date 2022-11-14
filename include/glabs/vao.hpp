#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <cpputils/types.hpp>

namespace GL {

struct VAO {
    u32 m_id;
    
    VAO();
    VAO(u32 id);
    VAO& create();
    VAO& destroy();
    VAO& use();
    VAO& unuse();
    ~VAO();
};

inline VAO::VAO() {
    create();
}

inline VAO::VAO(u32 id) : m_id(id) {

}

inline VAO& VAO::create() {
    glGenVertexArrays(1, &m_id);
    return *this;
}

inline VAO& VAO::destroy() {
    if (m_id) {
        glDeleteVertexArrays(1, &m_id);
        m_id = 0;
    }
    return *this;
}

inline VAO& VAO::use() {
    glBindVertexArray(m_id);
    return *this;
}

inline VAO& VAO::unuse() {
    glBindVertexArray(0);
    return *this;
}

inline VAO::~VAO() {
    destroy();
}

};

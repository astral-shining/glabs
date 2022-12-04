#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <cpputils/types.hpp>

namespace GL {

class VAO {
    u32 m_id;
    
public:
    VAO();
    VAO(u32 id);
    VAO& use();
    VAO& unuse();
    ~VAO();
};

inline VAO::VAO() {
    glGenVertexArrays(1, &m_id);
    logDebug("Created vao: %d", m_id);
}

inline VAO::VAO(u32 id) : m_id(id) {

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
    glDeleteVertexArrays(1, &m_id);
    logDebug("Destroyed vao: %d", m_id);
}

};

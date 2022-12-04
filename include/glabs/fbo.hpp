#pragma once
#include <glad/glad.h>
#include <cpputils/types.hpp>
#include <cpputils/debug.hpp>

class FBO {
    u32 m_id;
    
public:
    FBO();
    FBO(u32 id);
    FBO& use();
    FBO& unuse();
    ~FBO();
};


inline FBO::FBO() {
    glGenFramebuffers(1, &m_id);
    logDebug("Created fbo: %d", m_id);
}

inline FBO& FBO::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    return *this;
}

inline FBO& FBO::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return *this;
}

inline FBO::~FBO() {
    glDeleteFramebuffers(1, &m_id);
    logDebug("Destroyed fbo: %d", m_id);
}
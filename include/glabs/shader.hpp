#pragma once
#include <string_view>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <glad/glad.h>
#include <qwertyutils/types.hpp>

#include "vbo.hpp"


namespace GL {

class Shader;
class RGBA;
inline Shader* current_shader {};

class Shader {
    u32 m_vs;
    u32 m_fs;
    u32 m_program {};
    bool m_compiled {};
    std::string m_vSource;
    std::string m_fSource;

public:
    Shader(std::string vsource, std::string fsource);

    Shader& compile();
    Shader& use();
    Shader& use(auto&& lambda);
    Shader& unuse();

    u32 indexToLocation(u32 index);
    u32 getAttribLocation(std::string_view name) const;
    
    Shader& uniform(const char* name, int v);
    Shader& uniform(const char* name, const glm::vec2& v);
    Shader& uniform(const char* name, const glm::vec3& v);
    Shader& uniform(const char* name, const glm::vec4& v);
    Shader& uniform(const char* name, const glm::mat4& v);
    Shader& uniform(const char* name, const glm::mat3& v);
    Shader& uniform(const char* name, const glm::uvec2& v);
    Shader& uniform(const char* name, const RGBA v);
    Shader& uniform(const char* name, float v);
    
    template<std::size_t s = 0, typename... Ts>
    auto attribLinker(VBO<Ts...>& v);

    u32 getProgram() const;
    u32 getAttrib(const char* name) const;
    
    void destroy();
    ~Shader();
};

};

#include "attrib.hpp"

namespace GL {

inline Shader::Shader(std::string vsource, std::string fsource) : m_vSource(std::move(vsource)), m_fSource(std::move(fsource)) {
}

inline Shader& Shader::compile() {
    m_vs = glCreateShader(GL_VERTEX_SHADER);
    m_fs = glCreateShader(GL_FRAGMENT_SHADER);
    m_program = glCreateProgram();
    
    const char* ptr_vs = m_vSource.data();
    const char* ptr_fs = m_fSource.data();
    glShaderSource(m_vs, 1, &ptr_vs, NULL);
    glShaderSource(m_fs, 1, &ptr_fs, NULL);
    glCompileShader(m_vs);
    glCompileShader(m_fs);

    // Check errors
    int success;
    char infoLog[512];
    glGetShaderiv(m_vs, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(m_vs, 512, NULL, infoLog);
        throw std::runtime_error(std::string("ERROR: Compiling vertex shader ") + infoLog);
    }

    glGetShaderiv(m_fs, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(m_fs, 512, NULL, infoLog);
        throw std::runtime_error(std::string("ERROR: Compiling fragment shader ") + infoLog);
    }

    // Link shaders
    glAttachShader(m_program, m_vs);
    glAttachShader(m_program, m_fs);
    glLinkProgram(m_program);

    glDeleteShader(m_vs);
    glDeleteShader(m_fs);

    GLint count;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &count);
    for (int i {}; i < count; i++) {
        char name[16];
        GLenum type;
        int size;
        int length;
        glGetActiveAttrib(m_program, (GLuint)i, sizeof(name), &length, &size, &type, name);
        int location = glGetAttribLocation(m_program, name);
        logDebug("%s index: %d size: %d location: %d", name, i, size, location);
    }

    return *this;
}

inline Shader& Shader::use() {
    glUseProgram(m_program);
    current_shader = this;
    return *this;
}

inline Shader& Shader::use(auto&& lambda) {
    auto* current_s = current_shader;
    use();
    lambda(*this);
    if (current_s) current_s->use();
    else unuse();
    return *this;
}

inline Shader& Shader::unuse() {
    glUseProgram(0);
    return *this;
}

inline u32 Shader::indexToLocation(u32 index) {
    char name[16] {};
    GLenum type;
    GLint size;
    GLsizei length;

    glGetActiveAttrib(m_program, index, sizeof(name), &length, &size, &type, name);
    u32 location = glGetAttribLocation(m_program, name);
    logDebug("Index to location %d -> %d [%s]", index, location, name);
    return location;
}

inline u32 Shader::getAttribLocation(std::string_view name) const {
    return glGetAttribLocation(m_program, name.data());
}

inline Shader& Shader::uniform(const char* name, int v) {
    int location = glGetUniformLocation(m_program, name);
    glUniform1i(location, v);
    return *this;
}

inline Shader& Shader::uniform(const char* name, const glm::vec2& v) {
    int location = glGetUniformLocation(m_program, name);
    glUniform2fv(location, 1, reinterpret_cast<const float*>(&v));
    return *this;
}

inline Shader& Shader::uniform(const char* name, const glm::vec3& v) {
    int location = glGetUniformLocation(m_program, name);
    glUniform3fv(location, 1, reinterpret_cast<const float*>(&v));
    return *this;
}

inline Shader& Shader::uniform(const char* name, const glm::vec4& v) {
    int location = glGetUniformLocation(m_program, name);
    glUniform4fv(location, 1, reinterpret_cast<const float*>(&v));
    return *this;
}

inline Shader& Shader::uniform(const char* name, const glm::mat4& v) {
    int location = glGetUniformLocation(m_program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const float*>(&v));
    return *this;
}

inline Shader& Shader::uniform(const char* name, const glm::mat3& v) {
    int location = glGetUniformLocation(m_program, name);
    glUniformMatrix3fv(location, 1, GL_FALSE, reinterpret_cast<const float*>(&v));
    return *this;
}

inline Shader& Shader::uniform(const char* name, const glm::uvec2& v) {
    int location = glGetUniformLocation(m_program, name);
    glUniform2uiv(location, 1, reinterpret_cast<const u32*>(&v));
    return *this;
}

inline Shader& Shader::uniform(const char* name, float v) {
    int location = glGetUniformLocation(m_program, name);
    glUniform1f(location, v);
    return *this;
}

inline Shader& Shader::uniform(const char* name, const RGBA c) {
    glm::vec4 color {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f};
    uniform(name, color);
    return *this;
}

template<std::size_t s, typename... Ts>
inline auto Shader::attribLinker(VBO<Ts...>& vbo) {
    vbo.use();
    return AttribLinker<s, std::tuple<Ts...>>{*this};
}

inline u32 Shader::getAttrib(const char* name) const {
    return glGetAttribLocation(m_program, name);
}

inline void Shader::destroy() {
    if (m_compiled) {
        glDeleteProgram(m_program);
    }
}

inline Shader::~Shader() {
    destroy();
}

};

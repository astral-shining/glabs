#pragma once
#include <glad/glad.h>
#include <cpputils/types.hpp>
#include <cpputils/metafunctions.hpp>
#include <cpputils/constexpr_for.hpp>
#include <cppmaths/vec.hpp>
#include <cppmaths/mat.hpp>

extern "C" double fmod(double, double);
extern "C" double fabs(double);

namespace GL {

struct RGBA {
    u8 r, g, b, a;
    constexpr RGBA(u8 r = 0, u8 g = 0, u8 b = 0, u8 a = 0) : r(r), g(g), b(b), a(a) {
    }
};

struct RGB {
    u8 r, g, b;
    constexpr RGB(u8 r = 0, u8 g = 0, u8 b = 0) : r(r), g(g), b(b) {}
};

constexpr RGBA HexRGBA(u32 num) {
    return RGBA(
        (num & 0xff000000) >> 24u, 
        (num & 0x00ff0000) >> 16u, 
        (num & 0x0000ff00) >> 8u, 
        (num & 0x000000ff)
    );
}

constexpr RGBA HSVA(float H, float S, float V, float A = 1.f) {
    float C = S * V; // Chroma
    float HPrime = fmod(H / 60, 6.f); // H'
    float X = C * (1 - fabs(fmod(HPrime, 2.f) - 1));
    float M = V - C;

    float R = 0.f;
    float G = 0.f;
    float B = 0.f;

    switch (static_cast<int>(HPrime)) {
    case 0: R = C; G = X;        break; // [0, 1)
    case 1: R = X; G = C;        break; // [1, 2)
    case 2:        G = C; B = X; break; // [2, 3)
    case 3:        G = X; B = C; break; // [3, 4)
    case 4: R = X;        B = C; break; // [4, 5)
    case 5: R = C;        B = X; break; // [5, 6)
    }

    R += M;
    G += M;
    B += M;

    RGBA color;
    color.r = (u8) (R * 255);
    color.g = (u8) (G * 255);
    color.b = (u8) (B * 255);
    color.a = (u8) (A * 255);
    return color;
}

class Shader;

template<u32 N, typename Tupl>
class AttribLinker {
    Shader& m_shader;

    using type_N = TupleElementNoerror<N, Tupl>;

    template<u32 Advance>
    using R = Conditional<IsSame<TupleElementNoerror<N+Advance, Tupl>, void>, Shader&, AttribLinker<N+Advance, Tupl>>;

    using next_R = R<1>;

public:
    inline AttribLinker(Shader& shader) : m_shader(shader) {

    }

    template<typename T>
    inline void linkAttribute(u32 location, u32 start, u32 stride) {
        logDebug("attribute not found in %s", __PRETTY_FUNCTION__);
    }

    template<typename T>
    inline void linkInstancedAttribute(u32 location, u32 start, u32 stride, u32 divisor = 1) {
        linkAttribute<T>(location, start, stride);
        glVertexAttribDivisor(location, divisor);
    }

    template<IsSame<RGBA> T>
    inline void linkAttribute(u32 location, u32 start, u32 stride) {
        glVertexAttribPointer(location, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, reinterpret_cast<void*>(start));
        glEnableVertexAttribArray(location);
    }

    template<IsSame<RGB> T>
    inline void linkAttribute(u32 location, u32 start, u32 stride) {
        glVertexAttribPointer(location, 3, GL_UNSIGNED_BYTE, GL_TRUE, stride, reinterpret_cast<void*>(start));
        glEnableVertexAttribArray(location);
    }

    template<IsSame<Vec3> T>
    inline void linkAttribute(u32 location, u32 start, u32 stride) {
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(start));
        glEnableVertexAttribArray(location);
    }

    template<IsSame<Vec2> T>
    inline void linkAttribute(u32 location, u32 start, u32 stride) {
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(start));
        glEnableVertexAttribArray(location);
    }

    template<IsSame<Mat4> T>
    inline void linkAttribute(u32 location, u32 start, u32 stride) {
        glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(start));
        glVertexAttribPointer(location+1, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(start+16));
        glVertexAttribPointer(location+2, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(start+32));
        glVertexAttribPointer(location+3, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(start+48));
        glEnableVertexAttribArray(location);
        glEnableVertexAttribArray(location+1);
        glEnableVertexAttribArray(location+2);
        glEnableVertexAttribArray(location+3);
    }

    template<IsSame<Mat4> T>
    inline void linkInstancedAttribute(u32 location, u32 start, u32 stride, u32 divisor) {
        linkAttribute<Mat4>(location, start, stride);
        glVertexAttribDivisor(location, divisor);
        glVertexAttribDivisor(location+1, divisor);
        glVertexAttribDivisor(location+2, divisor);
        glVertexAttribDivisor(location+3, divisor);
    }

    template<IsSame<Mat3> T>
    inline void linkAttribute(u32 location, u32 start, u32 stride) {
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, start, reinterpret_cast<void*>(stride));
        glVertexAttribPointer(location+1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(start+12));
        glVertexAttribPointer(location+2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(start+24));
        glEnableVertexAttribArray(location);
        glEnableVertexAttribArray(location+1);
        glEnableVertexAttribArray(location+2);
    }

    template<IsSame<Mat3> T>
    inline void linkInstancedAttribute(u32 location, u32 start, u32 stride, u32 divisor) {
        linkAttribute<Mat3>(location, start, stride);
        glVertexAttribDivisor(location, divisor);
        glVertexAttribDivisor(location+1, divisor);
        glVertexAttribDivisor(location+2, divisor);
    }

    next_R linkAttribute(const char* name);
    next_R linkInstancedAttribute(const char* name);
    next_R skipAttribute();
    next_R autoLink();
    next_R autoInstancedLink();

    template<typename... Ts>
    next_R customLink(Ts&&...);

    template<u32 S>
    R<S> linkAttributes(const char* const (&)[S]);

    template<typename... Ts>
    next_R customInstancedLink(Ts&&...);

    Shader& autoLinkAll();
    inline Shader& end() {
        return m_shader;
    }

    inline Shader& getShader() {
        return m_shader;
    }
};

};

#include "shader.hpp"

namespace GL {

template<u32 N, typename Tupl>
inline typename AttribLinker<N, Tupl>::next_R AttribLinker<N, Tupl>::linkAttribute(const char* name) {
    linkAttribute<type_N>(m_shader.getAttribLocation(name), tupleOffset<N, Tupl>(), sizeof(Tupl));
    return m_shader;
}

template<u32 N, typename Tupl>
inline typename AttribLinker<N, Tupl>::next_R AttribLinker<N, Tupl>::linkInstancedAttribute(const char* name) {
    linkInstancedAttribute<type_N>(m_shader.getAttribLocation(name), tupleOffset<N, Tupl>(), sizeof(Tupl));
    return m_shader;
}

template<u32 N, typename Tupl>
inline typename AttribLinker<N, Tupl>::next_R AttribLinker<N, Tupl>::skipAttribute() {
    return m_shader;
}

template<u32 N, typename Tupl>
inline typename AttribLinker<N, Tupl>::next_R AttribLinker<N, Tupl>::autoLink() {
    linkAttribute<type_N>(m_shader.indexToLocation(N), tupleOffset<N, Tupl>(), sizeof(Tupl));
    return m_shader;
}

template<u32 N, typename Tupl>
inline typename AttribLinker<N, Tupl>::next_R AttribLinker<N, Tupl>::autoInstancedLink() {
    return linkInstancedAttribute<type_N>(m_shader.indexToLocation(N), tupleOffset<N, Tupl>(), sizeof(Tupl));
}

template<u32 N, typename Tupl>
template<typename... Ts>
inline typename AttribLinker<N, Tupl>::next_R AttribLinker<N, Tupl>::customLink(Ts&&... args) {
    type_N::linkAttributes(*this, forward<Ts>(args)...);
    return m_shader;
}

template<u32 N, typename Tupl>
template<typename... Ts>
inline typename AttribLinker<N, Tupl>::next_R AttribLinker<N, Tupl>::customInstancedLink(Ts&&... args) {
    type_N::linkInstancedAttributes(*this, forward<Ts>(args)...);
    return m_shader;
}

template<u32 N, typename Tupl>
template<u32 S>
inline typename AttribLinker<N, Tupl>::template R<S> AttribLinker<N, Tupl>::linkAttributes(const char* const (&n)[S]) {
    constexpr_for(u32 i=0, i<S, i+1, 
        using iT = TupleElement<i+N, Tupl>;
        linkAttribute<iT>(m_shader.getAttribLocation(n[i]), tupleOffset<i+N, Tupl>(), sizeof(Tupl));
    );
    return m_shader;
}

template<u32 N, typename Tupl>
inline Shader& AttribLinker<N, Tupl>::autoLinkAll() {
    constexpr_for(u32 i=N, i<TupleSize<Tupl>, i+1,
        using iT = TupleElement<i, Tupl>;
        linkAttribute<iT>(m_shader.indexToLocation(i), tupleOffset<i, Tupl>(), sizeof(Tupl));
    );
    return m_shader;
}

};


constexpr GL::RGBA operator""_rgba(unsigned long long int c) {
    return GL::HexRGBA(c);
}

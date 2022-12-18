#pragma once
#include <glad/glad.h>
#include <cpputils/types.hpp>
#include <cpputils/debug.hpp>
#include <cpputils/vector.hpp>
#include <cpputils/tuple.hpp>
#include <cpputils/metafunctions.hpp>

namespace GL {

template<typename T, typename... Ts>
class VBO {
    u32 m_id;

public:
    using type = Conditional<(sizeof...(Ts)>0), Tuple<T, Ts...>, T>;
    static constexpr std::size_t ntypes = 1+sizeof...(Ts);
    
    inline VBO() {
        glGenBuffers(1, &m_id);
        logDebug("Created vbo: %d", m_id);
    }

    inline VBO(std::initializer_list<type> l, u32 draw_type = GL_STATIC_DRAW) {
        glGenBuffers(1, &m_id);
        logDebug("Created vbo: %d", m_id);
        use();
        bufferData(l, draw_type);
    }

    inline auto& use() {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        return *this;
    }
    inline auto& unuse() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return *this;
    }

    template<typename T2>
    requires requires(T2 t) { t.data(); t.size(); }
    inline auto& bufferData(T2& data, u32 draw_type) {
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(type), data.data(), draw_type);
        return *this;
    }

    inline auto& bufferData(std::initializer_list<type> data, u32 draw_type) {
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(type), (void*)data.begin(), draw_type);
        return *this;
    }

    inline auto& bufferData(T* data, u32 size, u32 draw_type) {
        glBufferData(GL_ARRAY_BUFFER, size, (void*)data, draw_type);
        return *this;
    }

    inline auto& bufferSubData(Vector<type>& data, u32 offset, u32 size) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data.data());
        return *this;
    }

    inline auto& bufferSubData(T&& data, u32 offset) {
        glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(type), sizeof(type), &data);
    }

    // template<std::size_t... s>
    // requires (is_tuple_v<type> && sizeof...(s) > 0)
    // inline void bufferSubData(element_tuple<s, type>& data, int offset) {
    //     VBO_impl::bufferSubData(&data, sizeof(element_tuple<s, type>), offset*sizeof(type)+byteoffset_index_tuple_v<(s + ...), unpack_tuples<type>>);
    // }

    template<typename T2>
    requires (IsTuple<type>)
    inline auto& bufferSubData(T2&& data, u32 offset) {
        using T2_noref = RemoveReference<T2>;
    //    logDebug("bufferSubData offset: %d, size %d", offset*sizeof(type)+tuple_offset<T2_noref, type>(), sizeof(T2_noref));
        glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(type)+tupleOffset<T2_noref, type>(), sizeof(T2_noref), &data);
        return *this;

    }
    
    inline auto& bufferSubData(T* data, u32 size, u32 offset) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        return *this;
    }

    inline ~VBO() {
        glDeleteBuffers(1, &m_id);
        logDebug("Destroyed vbo: %d", m_id);
    }
};

};

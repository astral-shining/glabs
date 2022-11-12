#pragma once
#include <cstdint>
#include <vector>
#include <tuple>
#include <span>
#include <initializer_list>
#include <type_traits>
#include <glad/glad.h>
#include <qwertyutils/types.hpp>
#include <qwertyutils/debug.hpp>
#include <qwertyutils/tupleutils.hpp>

namespace GL {

#define DYNAMIC_DRAW 0x88E8
#define STATIC_DRAW 0x88E4

template<typename T, typename... Ts>
using vbo_type = std::conditional_t<(sizeof...(Ts)>0), std::tuple<T, Ts...>, T>;

template<typename T, typename... Ts>
class VBO {
    u32 m_id;

public:
    using type = vbo_type<T, Ts...>;
    static constexpr std::size_t ntypes = 1+sizeof...(Ts);
    
    VBO();
    VBO(std::initializer_list<type> l, u32 draw_type = STATIC_DRAW);

    void use();
    void unuse();

    template<typename T2>
    requires requires(T2 t) { t.data(); t.size(); }
    void bufferData(T2& data, u32 draw_type);
    void bufferData(std::initializer_list<type> data, u32 draw_type);
    void bufferData(T* data, u32 size, u32 draw_type);
    void bufferSubData(std::vector<type>& data, u32 offset, u32 size);
    void bufferSubData(T&& data, u32 offset);

    // template<std::size_t... s>
    // requires (is_tuple_v<type> && sizeof...(s) > 0)
    // inline void bufferSubData(element_tuple<s, type>& data, int offset) {
    //     VBO_impl::bufferSubData(&data, sizeof(element_tuple<s, type>), offset*sizeof(type)+byteoffset_index_tuple_v<(s + ...), unpack_tuples<type>>);
    // }

    template<typename T2>
    requires (is_tuple_v<type>)
    void bufferSubData(T2&& data, u32 offset);
    void bufferSubData(T* data, u32 size, u32 offset);

    ~VBO();
};

template<typename T, typename... Ts>
inline VBO<T, Ts...>::VBO() {
    glGenBuffers(1, &m_id);
    logDebug("Created vbo: %d", m_id);
}

template<typename T, typename... Ts>
inline VBO<T, Ts...>::VBO(std::initializer_list<vbo_type<T, Ts...>> l, u32 draw_type) {
    glGenBuffers(1, &m_id);
    logDebug("Created vbo: %d", m_id);
    use();
    bufferData(l, draw_type);
}

template<typename T, typename... Ts>
inline void VBO<T, Ts...>::use() {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

template<typename T, typename... Ts>
inline void VBO<T, Ts...>::unuse() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T, typename... Ts>
template<typename T2>
requires requires(T2 t) { t.data(); t.size(); }
void VBO<T, Ts...>::bufferData(T2& data, u32 draw_type) {
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vbo_type<T, Ts...>), data.data(), draw_type);
}

template<typename T, typename... Ts>
inline void VBO<T, Ts...>::bufferData(std::initializer_list<vbo_type<T, Ts...>> data, u32 draw_type) {
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vbo_type<T, Ts...>), (void*)data.begin(), draw_type);
}

template<typename T, typename... Ts>
inline void VBO<T, Ts...>::bufferData(T* data, u32 size, u32 draw_type) {
    glBufferData(GL_ARRAY_BUFFER, size, (void*)data, draw_type);
}

template<typename T, typename... Ts>
inline void VBO<T, Ts...>::bufferSubData(std::vector<vbo_type<T, Ts...>>& data, u32 offset, u32 size) {
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data.data());
}

template<typename T, typename... Ts>
inline void VBO<T, Ts...>::bufferSubData(T&& data, u32 offset) {
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(type), sizeof(type), &data);
}

template<typename T, typename... Ts>
template<typename T2>
requires (is_tuple_v<vbo_type<T, Ts...>>)
void VBO<T, Ts...>::bufferSubData(T2&& data, u32 offset) {
    using T2_noref = std::remove_reference_t<T2>;
//    logDebug("bufferSubData offset: %d, size %d", offset*sizeof(type)+tuple_offset<T2_noref, type>(), sizeof(T2_noref));
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(type)+tuple_offset<T2_noref, type>(), sizeof(T2_noref), &data);
}

template<typename T, typename... Ts>
inline void VBO<T, Ts...>::bufferSubData(T* data, u32 size, u32 offset) {
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

template<typename T, typename... Ts>
inline VBO<T, Ts...>::~VBO() {
    logDebug("Destroyed vbo: %d", m_id);
    glDeleteBuffers(1, &m_id);
}

};

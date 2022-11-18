#pragma once
#include <glad/glad.h>

#include "shader.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "attrib.hpp"

namespace GL {

// inline VBO<glm::vec3> square_vbo {
//     {-1.0, -1.0, 0.0}, // Bottom left
//     {1.0, -1.0, 0.0}, // Bottom right
//     {1.0, 1.0, 0.0}, // Top right
//     {-1.0, 1.0, 0.0}  // Top left
// };

// inline VBO<glm::vec2, glm::vec2> square_texture_vbo {
//     {{-1.0, -1.0},{0.0, 0.0}}, // Bottom left
//     {{1.0, -1.0}, {1.0, 0.0}}, // Bottom right
//     {{1.0, 1.0},  {1.0, 1.0}}, // Top right
//     {{-1.0, 1.0}, {0.0, 1.0}}  // Top left
// };

// inline VBO<glm::vec2> texture_vbo {
//     {0.0, 0.0}, // Bottom left
//     {1.0, 0.0}, // Bottom right
//     {1.0, 1.0}, // Top right
//     {0.0, 1.0}  // Top left
// };

// template<RGB c1, RGB c2, RGB c3, RGB c4>
// inline VBO<glm::vec2, RGB> square_color_vbo {
//     {{-1.0, -1.0},c1}, // Bottom left
//     {{1.0, -1.0}, c2}, // Bottom right
//     {{1.0, 1.0},  c3}, // Top right
//     {{-1.0, 1.0}, c4}  // Top left
// };

inline void draw_square() {
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

inline void drawSquareInstanced(u32 count) {
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, count);    
}

};

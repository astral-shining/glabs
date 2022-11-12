#pragma once
#include <glad/glad.h>
#include <cpputils/debug.hpp>

namespace GL {

template<typename T>
inline void load(T* (*addr) (const char*)) { 
    logDebug("Initializing glad"); 
    if (!gladLoadGLLoader((GLADloadproc) addr)) {
        throw std::runtime_error("ERROR: Initializing glad");
    }

    logDebug("Status: Using OpenGL Core 3.3");
}

inline void unload() {
//    gladLoaderUnloadGL();
}

}

// glfwGetProcAddress
//glEnable(GL_DEPTH_TEST);                                  
//glDepthFunc(GL_LEQUAL);                                    
//loadFTLib();
// glShadeModel(GL_SMOOTH);
//glEnable(GL_BLEND);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
// glEnable(GL_TEXTURE_2D);

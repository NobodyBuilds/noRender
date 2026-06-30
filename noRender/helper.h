#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include<glad/glad.h>
#include <cstdint>
#include<vector>
#include <cmath>
#include <vector_types.h>

struct vec2 {
    float x;
    float y;
};
static GLuint compileShader(GLenum type, const char* src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char buf[1024];
        glGetShaderInfoLog(s, 1024, nullptr, buf);
        std::cerr << "Shader compile error: " << buf << "\n";
    }
    return s;
}


static GLuint createProgram(const char* vs, const char* fs)
{
    GLuint a = compileShader(GL_VERTEX_SHADER, vs);
    GLuint b = compileShader(GL_FRAGMENT_SHADER, fs);
    GLuint p = glCreateProgram();
    glAttachShader(p, a);
    glAttachShader(p, b);
    glLinkProgram(p);
    GLint ok;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char buf[1024];
        glGetProgramInfoLog(p, 1024, nullptr, buf);
        std::cerr << "Program link error: " << buf << "\n";
    }
    glDeleteShader(a);
    glDeleteShader(b);
    return p;
    
}

inline vec2 PixelToNDC(float pixelX, float pixelY, float windowWidth, float windowHeight) {
    vec2 ndc;
    ndc.x = (pixelX / windowWidth) * 2.0f - 1.0f;
    ndc.y = (pixelY / windowHeight) * 2.0f - 1.0f;
    return ndc;
}



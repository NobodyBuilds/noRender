#include "norender.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderdata.h"
#include "helper.h"
#include <vector>
#include "buffers.h"

void Render2d::drawtriangle(float x, float y, float r, float g, float b, float size,float rotation)
{

    if (noRender.getMode() == 3)
    {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene \n");
            error = false;
        }
        return;
    }

    static bool firstcall = true;
    if (firstcall)
    {
        initTriangleBuffer2d();
        firstcall = false;
    }
   
    float s = size;
    float radian = rotation * (3.14159265f / 180.0f);
    float c = cos(radian);
    float S = sin(radian);

    
    float half_width = s;
    float height_up = s * 1.1547f;   // Distance from center to top vertex
    float height_down = s * 0.5773f; // Distance from center to bottom base

    float n1x_local = -half_width * c - (-height_down) * S;
    float n1y_local = -half_width * S + (-height_down) * c;

    float n2x_local = 0 * c - height_up * S;
    float n2y_local = 0 * S + height_up * c;

    float n3x_local = half_width * c - (-height_down) * S;
    float n3y_local = half_width * S + (-height_down) * c;

    vec2 n1 = PixelToNDC(x + n1x_local, y + n1y_local, noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n2 = PixelToNDC(x + n2x_local, y + n2y_local, noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n3 = PixelToNDC(x + n3x_local, y + n3y_local, noRender.getscreenwidth(), noRender.getscreenheight());

    float vertices[] = {
        n1.x, n1.y,
        n2.x, n2.y,
        n3.x, n3.y
    };
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glUseProgram(triangleprogram);
    glUniform3f(glGetUniformLocation(triangleprogram, "col"), r, g, b);

    glBindVertexArray(triVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
void Render2d::drawcircle(float x, float y, float r, float g, float b, float size)
{
    if (noRender.getMode() == 3)
    {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }
    static bool firstcall = true;
    if (firstcall)
    {
        initCircleBuffer2d();
        firstcall = false;
    }
    float s = size;
    vec2 n1 = PixelToNDC(x - s, y - s, noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n2 = PixelToNDC(x + 3.0f * s, y - s, noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n3 = PixelToNDC(x - s, y + 3.0f * s, noRender.getscreenwidth(), noRender.getscreenheight());
    float ox[3] = {-1.0f, 3.0f, -1.0f};
    float oy[3] = {-1.0f, -1.0f, 3.0f};

    float vertices[] = {
        n1.x, n1.y, ox[0], oy[0],
        n2.x, n2.y, ox[1], oy[1],
        n3.x, n3.y, ox[2], oy[2]};

    glBindBuffer(GL_ARRAY_BUFFER, cirVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glUseProgram(circleprogram);
    glUniform3f(glGetUniformLocation(circleprogram, "col"), r, g, b);
    glBindVertexArray(cirVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
void Render2d::drawquad(float x, float y, float r, float g, float b, float width, float height,float rotation)
{
    if (noRender.getMode() == 3)
    {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }
    static bool firstcall = true;
    if (firstcall)
    {
        initQuadBuffer2d();
        firstcall = false;
    }
    float radian = rotation * (3.14f / 180.0f);
    float c = cos(radian);
    float S = sin(radian);
    float w = width;
    float h = height;

    float lx1 = -w * c - h * S;
    float ly1 = -w * S + h * c;

    float lx2 = -w * c  - (-h) * S;
    float ly2 = -w * S + (-h) * c;

    float lx3 = w * c - (-h) * S;
    float ly3 = w * S + (-h) * c;

    float lx4 = -w * c - h * S;
    float ly4 = -w * S + h * c;

    float lx5 = w * c - h * S;
    float ly5 = w * S + h * c;

    float lx6 = w * c - (-h) * S;
    float ly6 = w * S + (-h) * c;


   

    vec2 n1 = PixelToNDC(x + lx1, y +ly1, noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n2 = PixelToNDC(x + lx2, y +ly2, noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n3 = PixelToNDC(x + lx3, y +ly3, noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n4 = PixelToNDC(x + lx4, y +ly4,noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n5 = PixelToNDC(x + lx5, y +ly5,noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n6 = PixelToNDC(x + lx6, y +ly6,noRender.getscreenwidth(), noRender.getscreenheight());

    float vertices[] = {
        n1.x, n1.y,
        n2.x, n2.y,
        n3.x, n3.y,
        n4.x, n4.y,
        n5.x, n5.y,
        n6.x, n6.y };

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glUseProgram(quadprogram);
    glUniform3f(glGetUniformLocation(quadprogram, "col"), r, g, b);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void Render2d::drawtriangleinstanced(std::vector<Trianglevertex2d> &instances)
{
    if (noRender.getMode() == 3)
    {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }
    static bool firstcall = true;
    if (firstcall)
    {
        initInstancedTriangleBuffer2d();
        firstcall = false;
    }
    int count = (int)instances.size();
    if (count == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, triInstDataVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 7 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(triInstProgram);
    glUniform2f(glGetUniformLocation(triInstProgram, "screenSize"),
                (float)noRender.getscreenwidth(), (float)noRender.getscreenheight());

    glBindVertexArray(triInstVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}
void Render2d::drawQuadinstanced(std::vector<quadvertex2d> &instances)
{
    if (noRender.getMode() == 3)
    {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }
    static bool firstcall = true;
    if (firstcall)
    {
        initInstancedQuadBuffer2d();
        firstcall = false;
    }
    int count = (int)instances.size();
    if (count == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, quadInstDataVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 8 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(quadInstProgram);
    glUniform2f(glGetUniformLocation(quadInstProgram, "screenSize"),
                (float)noRender.getscreenwidth(), (float)noRender.getscreenheight());

    glBindVertexArray(quadInstVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
    glBindVertexArray(0);
}
void Render2d::drawcircleinstanced(std::vector<circlevertex2d> &instances)
{
    if (noRender.getMode() == 3)
    {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }
    static bool firstcall = true;
    if (firstcall)
    {
        initInstancedCircleBuffer2d();
        firstcall = false;
    }
    int count = (int)instances.size();
    if (count == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, circleInstDataVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 6 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(circleInstProgram);
    glUniform2f(glGetUniformLocation(circleInstProgram, "screenSize"),
                (float)noRender.getscreenwidth(), (float)noRender.getscreenheight());

    glBindVertexArray(circleInstVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}
void Render2d::drawline(float x1, float y1, float x2, float y2, float r, float g, float b)
{
    if (noRender.getMode() == 3)
    {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }
    static bool firstcall = true;
    if (firstcall)
    {
        initLineBuffer2d();
        firstcall = false;
    }
    vec2 n1 = PixelToNDC(x1, y1, noRender.getscreenwidth(), noRender.getscreenheight());
    vec2 n2 = PixelToNDC(x2, y2, noRender.getscreenwidth(), noRender.getscreenheight());

    float vertices[] = {
        n1.x, n1.y, r, g, b,
        n2.x, n2.y, r, g, b};

    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glUseProgram(lineprogram);
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}
void Render2d::drawlinechain(std::vector<chainpoint2d> &points, float r, float g, float b)
{
    if (noRender.getMode() == 3)
    {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }

    static bool firstcall = true;
    if (firstcall)
    {
        initChainBuffer2d();
        firstcall = false;
    }
    int count = (int)points.size();
    if (count < 2)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, chainVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 2 * sizeof(float), points.data(), GL_STREAM_DRAW);

    glUseProgram(chainprogram);
    glUniform2f(glGetUniformLocation(chainprogram, "screenSize"),
                (float)noRender.getscreenwidth(), (float)noRender.getscreenheight());
    glUniform3f(glGetUniformLocation(chainprogram, "linecolor"), r, g, b);

    glBindVertexArray(chainVAO);
    glDrawArrays(GL_LINE_STRIP, 0, count);
    glBindVertexArray(0);
}
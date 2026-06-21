#include "norender.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"renderdata.h"
#include "helper.h"
#include <vector>
#include "buffers.h"



void Render2d::drawtriangle(float x ,float y, float r,float g,float b,float size ) {
    
    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error) { printf("ERROR: use render3D class instead of render2D for 3D scene"); error = false; }
        return;
  }

    static bool firstcall = true; 
    if (firstcall) {
        initTriangleBuffer2d();
        firstcall = false;
       
    }
    float s = size;
    float2 n1 = PixelToNDC(x - s, y - s, noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n2 = PixelToNDC(x, y + s,     noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n3 = PixelToNDC(x + s, y - s, noRender.getscreenwidth(), noRender.getscreenheight());

    float vertices[] = {
        n1.x, n1.y, r, g, b,
        n2.x, n2.y, r, g, b,
        n3.x, n3.y, r, g, b
    };

    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glUseProgram(triangleprogram);
    glBindVertexArray(triVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

}
void Render2d::drawcircle(float x ,float y, float r,float g,float b,float size ) {
    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error) { printf("ERROR: use render3D class instead of render2D for 3D scene"); error = false; }
        return;
    }
    static bool firstcall = true;
    if (firstcall) {
        initCircleBuffer2d();
        firstcall = false;

    }
    float s = size;
    float2 n1 = PixelToNDC(x - s, y - s, noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n2 = PixelToNDC(x+3.0f*s, y - s, noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n3 = PixelToNDC(x - s, y +3.0f* s, noRender.getscreenwidth(), noRender.getscreenheight());
    float ox[3] = { -1.0f, 3.0f, -1.0f };
    float oy[3] = { -1.0f, -1.0f, 3.0f };

    float vertices[] = {
        n1.x, n1.y, r, g, b,ox[0],oy[0],
        n2.x, n2.y, r, g, b,ox[1],oy[1],
        n3.x, n3.y, r, g, b,ox[2],oy[2]
    };

    glBindBuffer(GL_ARRAY_BUFFER, cirVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glUseProgram(circleprogram);
    glBindVertexArray(cirVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

}
void Render2d::drawquad(float x ,float y, float r,float g,float b,float width,float height ) {
    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error) { printf("ERROR: use render3D class instead of render2D for 3D scene"); error = false; }
        return;
    }
    static bool firstcall = true;
    if (firstcall) {
        initQuadBuffer2d();
        firstcall = false;

    }

    float2 n1 = PixelToNDC(x - width/2, y + height/2 , noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n2 = PixelToNDC(x - width/2, y - height/2, noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n3 = PixelToNDC(x + width/2, y - height/2, noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n4 = PixelToNDC(x - width/2, y + height/2, noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n5 = PixelToNDC(x + width/2, y + height/2, noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n6 = PixelToNDC(x + width/2, y - height/2, noRender.getscreenwidth(), noRender.getscreenheight());

    float vertices[] = {
        n1.x, n1.y, r, g, b,
        n2.x, n2.y, r, g, b,
        n3.x, n3.y, r, g, b,
        n4.x, n4.y, r, g, b,
        n5.x, n5.y, r, g, b,
        n6.x, n6.y, r, g, b
    };

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glUseProgram(quadprogram);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

}
void Render2d::drawtriangleinstanced(std::vector<Trianglevertex>& instances) {
    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error) { printf("ERROR: use render3D class instead of render2D for 3D scene"); error = false; }
        return;
    }
    static bool firstcall = true;
    if (firstcall) {
        initInstancedTriangleBuffer2d();
        firstcall = false;

    }
    int count = (int)instances.size();
    if (count == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, triInstDataVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 6 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(triInstProgram);
    glUniform2f(glGetUniformLocation(triInstProgram, "screenSize"),
        (float)noRender.getscreenwidth(), (float)noRender.getscreenheight());

    glBindVertexArray(triInstVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}
void Render2d::drawcircleinstanced(std::vector<circlevertex>& instances) {
    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error) { printf("ERROR: use render3D class instead of render2D for 3D scene"); error = false; }
        return;
    }
    static bool firstcall = true;
    if (firstcall) {
        initInstancedCircleBuffer2d();
        firstcall = false;

    }
    int count = (int)instances.size();
    if (count == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, circleInstDataVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 6 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(circleInstProgram);
    glUniform2f(glGetUniformLocation(circleInstProgram, "screenSize"),
        (float)noRender.getscreenwidth(), (float)noRender.getscreenheight());

    glBindVertexArray(circleInstVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}
void Render2d::drawQuadinstanced(std::vector<quadvertex>& instances) {
    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error) { printf("ERROR: use render3D class instead of render2D for 3D scene"); error = false; }
        return;
    }
    static bool firstcall = true;
    if (firstcall) {
        initInstancedQuadBuffer2d();
        firstcall = false;

    }
    int count = (int)instances.size();
    if (count == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, quadInstDataVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 7 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(quadInstProgram);
    glUniform2f(glGetUniformLocation(quadInstProgram, "screenSize"),
        (float)noRender.getscreenwidth(), (float)noRender.getscreenheight());

    glBindVertexArray(quadInstVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
    glBindVertexArray(0);
}
void Render2d::drawline(float x1, float y1, float x2, float y2, float r, float g, float b) {
    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error) { printf("ERROR: use render3D class instead of render2D for 3D scene"); error = false; }
        return;
    }
    static bool firstcall = true;
    if (firstcall) {
        initLineBuffer2d();
        firstcall = false;

    }
    float2 n1 = PixelToNDC(x1, y1, noRender.getscreenwidth(), noRender.getscreenheight());
    float2 n2 = PixelToNDC(x2, y2, noRender.getscreenwidth(), noRender.getscreenheight());

    float vertices[] = {
        n1.x, n1.y, r, g, b,
        n2.x, n2.y, r, g, b
    };

    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glUseProgram(lineprogram);
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}
void Render2d::drawlinechain(std::vector<chainpoint>& points, float r, float g, float b) {
    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error) { printf("ERROR: use render3D class instead of render2D for 3D scene"); error = false; }
        return;
    }

    static bool firstcall = true;
    if (firstcall) {
        initChainBuffer2d();
        firstcall = false;

    }
    int count = (int)points.size();
    if (count < 2) return;

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
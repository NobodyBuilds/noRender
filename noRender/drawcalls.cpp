#include "norender.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"renderdata.h"
#include "helper.h"
#include <vector>



void norender::drawtriangle(float x ,float y, float r,float g,float b,float size ) {
    float s = size;
    float2 n1 = PixelToNDC(x - s, y - s, d.screenwidth, d.screeheight);
    float2 n2 = PixelToNDC(x, y + s, d.screenwidth, d.screeheight);
    float2 n3 = PixelToNDC(x + s, y - s, d.screenwidth, d.screeheight);

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
void norender::drawcircle(float x ,float y, float r,float g,float b,float size ) {
    float s = size;
    float2 n1 = PixelToNDC(x - s, y - s, d.screenwidth, d.screeheight);
    float2 n2 = PixelToNDC(x+3.0f*s, y - s, d.screenwidth, d.screeheight);
    float2 n3 = PixelToNDC(x - s, y +3.0f* s, d.screenwidth, d.screeheight);
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
void norender::drawquad(float x ,float y, float r,float g,float b,float width,float height ) {

    float2 n1 = PixelToNDC(x - width/2, y + height/2 , d.screenwidth, d.screeheight);
    float2 n2 = PixelToNDC(x - width/2, y - height/2, d.screenwidth, d.screeheight);
    float2 n3 = PixelToNDC(x + width/2, y - height/2, d.screenwidth, d.screeheight);
    float2 n4 = PixelToNDC(x - width/2, y + height/2, d.screenwidth, d.screeheight);
    float2 n5 = PixelToNDC(x + width/2, y + height/2, d.screenwidth, d.screeheight);
    float2 n6 = PixelToNDC(x + width/2, y - height/2, d.screenwidth, d.screeheight);

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
void norender::drawtriangleinstanced(std::vector<Trianglevertex>& instances) {
    int count = (int)instances.size();
    if (count == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, triInstDataVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 6 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(triInstProgram);
    glUniform2f(glGetUniformLocation(triInstProgram, "screenSize"),
        (float)d.screenwidth, (float)d.screeheight);

    glBindVertexArray(triInstVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}
void norender::drawcircleinstanced(std::vector<circlevertex>& instances) {
    int count = (int)instances.size();
    if (count == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, circleInstDataVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 6 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(circleInstProgram);
    glUniform2f(glGetUniformLocation(circleInstProgram, "screenSize"),
        (float)d.screenwidth, (float)d.screeheight);

    glBindVertexArray(circleInstVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}


void norender::drawQuadinstanced(std::vector<quadvertex>& instances) {
    int count = (int)instances.size();
    if (count == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, quadInstDataVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 7 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(quadInstProgram);
    glUniform2f(glGetUniformLocation(quadInstProgram, "screenSize"),
        (float)d.screenwidth, (float)d.screeheight);

    glBindVertexArray(quadInstVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
    glBindVertexArray(0);
}



void norender::drawline(float x1, float y1, float x2, float y2, float r, float g, float b) {
    float2 n1 = PixelToNDC(x1, y1, d.screenwidth, d.screeheight);
    float2 n2 = PixelToNDC(x2, y2, d.screenwidth, d.screeheight);

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

void norender::drawlinechain(std::vector<chainpoint>& points, float r, float g, float b) {
    int count = (int)points.size();
    if (count < 2) return;

    glBindBuffer(GL_ARRAY_BUFFER, chainVBO);
    glBufferData(GL_ARRAY_BUFFER, count * 2 * sizeof(float), points.data(), GL_STREAM_DRAW);

    glUseProgram(chainprogram);
    glUniform2f(glGetUniformLocation(chainprogram, "screenSize"),
        (float)d.screenwidth, (float)d.screeheight);
    glUniform3f(glGetUniformLocation(chainprogram, "linecolor"), r, g, b);

    glBindVertexArray(chainVAO);
    glDrawArrays(GL_LINE_STRIP, 0, count);  
    glBindVertexArray(0);
}
#include "norender.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"renderdata.h"
#include "helper.h"



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
void norender::drawquad(float x ,float y, float r,float g,float b,float size ) {
    float s = size;
    float2 n1 = PixelToNDC(x-s , y+s , d.screenwidth, d.screeheight);
    float2 n2 = PixelToNDC(x-s, y - s, d.screenwidth, d.screeheight);
    float2 n3 = PixelToNDC(x + s, y - s, d.screenwidth, d.screeheight);
    float2 n4 = PixelToNDC(x - s, y + s, d.screenwidth, d.screeheight);
    float2 n5 = PixelToNDC(x + s, y + s, d.screenwidth, d.screeheight);
    float2 n6 = PixelToNDC(x + s, y - s, d.screenwidth, d.screeheight);

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
void norender::drawrectangle(float x ,float y, float r,float g,float b,float size,float width,float height ) {
    float s = size;
    float2 n1 = PixelToNDC(x-(s+width) , y+(s+height) , d.screenwidth, d.screeheight);
    float2 n2 = PixelToNDC(x- (s + width), y - (s + height), d.screenwidth, d.screeheight);
    float2 n3 = PixelToNDC(x + (s + width), y - (s + height), d.screenwidth, d.screeheight);
    float2 n4 = PixelToNDC(x - (s + width), y + (s + height), d.screenwidth, d.screeheight);
    float2 n5 = PixelToNDC(x + (s + width), y + (s + height), d.screenwidth, d.screeheight);
    float2 n6 = PixelToNDC(x + (s + width), y - (s + height), d.screenwidth, d.screeheight);

    float vertices[] = {
        n1.x, n1.y, r, g, b,
        n2.x, n2.y, r, g, b,
        n3.x, n3.y, r, g, b,
        n4.x, n4.y, r, g, b,
        n5.x, n5.y, r, g, b,
        n6.x, n6.y, r, g, b
    };

    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glUseProgram(rectprogram);
    glBindVertexArray(rectVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

}

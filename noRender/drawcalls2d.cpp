#include "norender.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderdata.h"
#include "helper.h"
#include <vector>
#include "buffers.h"
#include "camera.h"



void Render::triangle(float x, float y, float r, float g, float b, float size,float rotation)
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

    vec2 n1 = PixelToNDC(x + n1x_local - view.cameraX + noRender.getScreenWidth() * 0.5f, y + n1y_local - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n2 = PixelToNDC(x + n2x_local - view.cameraX + noRender.getScreenWidth() * 0.5f, y + n2y_local - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n3 = PixelToNDC(x + n3x_local - view.cameraX + noRender.getScreenWidth() * 0.5f, y + n3y_local - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());

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
void Render::circle(float x, float y, float r, float g, float b, float size)
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
    float s = size*0.5f;
    vec2 n1 = PixelToNDC(x - s - view.cameraX + noRender.getScreenWidth() * 0.5f, y - s - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n2 = PixelToNDC(x + 3.0f * s - view.cameraX + noRender.getScreenWidth() * 0.5f, y - s - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n3 = PixelToNDC(x - s - view.cameraX + noRender.getScreenWidth() * 0.5f, y + 3.0f * s - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
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
void Render::quad(float x, float y, float r, float g, float b, float width, float height,float rotation)
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
    float w = width*0.5f;
    float h = height*0.5f;

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


   

    vec2 n1 = PixelToNDC(x + lx1 - view.cameraX + noRender.getScreenWidth() * 0.5f, y + ly1 - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n2 = PixelToNDC(x + lx2 - view.cameraX + noRender.getScreenWidth() * 0.5f, y + ly2 - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n3 = PixelToNDC(x + lx3 - view.cameraX + noRender.getScreenWidth() * 0.5f, y + ly3 - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n4 = PixelToNDC(x + lx4 - view.cameraX + noRender.getScreenWidth() * 0.5f, y + ly4 - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n5 = PixelToNDC(x + lx5 - view.cameraX + noRender.getScreenWidth() * 0.5f, y + ly5 - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n6 = PixelToNDC(x + lx6 - view.cameraX + noRender.getScreenWidth() * 0.5f, y + ly6 - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());

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
void Render::triangleBatch(std::vector<Trianglevertex2d> &instances)
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
    int count = (int)instances.size();
    if (count == 0)
        return;
    static int oldcount = 0;
    static bool firstcall = true;
    if (firstcall || count != oldcount)
    {
        initInstancedTriangleBuffer2d(count,0);
        firstcall = false;
        oldcount = count;
    }

    glBindBuffer(GL_ARRAY_BUFFER, triInstDataVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, count * 7 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(triInstProgram);
    glUniform2f(glGetUniformLocation(triInstProgram, "screenSize"),
                (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(triInstProgram, "cameraPos2D"), view.cameraX, view.cameraY);

    glBindVertexArray(triInstVAO[0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}
void Render::quadBatch(std::vector<quadvertex2d> &instances)
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
    int count = (int)instances.size();
    if (count == 0)
        return;
    static int oldcount = 0;
    static bool firstcall = true;
    if (firstcall || count != oldcount)
    {
        initInstancedQuadBuffer2d(count,0);
        firstcall = false;
        oldcount = count;
    }

    glBindBuffer(GL_ARRAY_BUFFER, quadInstDataVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, count * 8 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(quadInstProgram);
    glUniform2f(glGetUniformLocation(quadInstProgram, "screenSize"),
                (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(quadInstProgram, "cameraPos2D"), view.cameraX, view.cameraY);

    glBindVertexArray(quadInstVAO[0]);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);
    glBindVertexArray(0);
}
void Render::quadBatchInterop(int count,int id) {//for interops ,no data loading only drawing

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
    static int oldcount[max_interop] = {};
    static bool initialized[max_interop] = {};
    if (!initialized[id] || count != oldcount[id])
    {
        initInstancedQuadBuffer2d(count,id);
        initialized[id] = true;
        oldcount[id] = count;
    }
   
    if (count == 0)
        return;
    glBindBuffer(GL_ARRAY_BUFFER, quadInstDataVBO[id]);
    glUseProgram(quadInstProgram);
    glUniform2f(glGetUniformLocation(quadInstProgram, "screenSize"),
        (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(quadInstProgram, "cameraPos2D"), view.cameraX, view.cameraY);

    glBindVertexArray(quadInstVAO[id]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
    glBindVertexArray(0);
}
void Render::circleBatch(std::vector<circlevertex2d> &instances)
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
    int count = (int)instances.size();
    static int oldcount = 0;
    static bool firstcall = true;

    if (firstcall || count != oldcount)
    {
        initInstancedCircleBuffer2d(count,0);
        firstcall = false;
        oldcount = count;
    }
    if (count == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, circleInstDataVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, count * 6 * sizeof(float), instances.data(), GL_STREAM_DRAW);

    glUseProgram(circleInstProgram);
    glUniform2f(glGetUniformLocation(circleInstProgram, "screenSize"),
                (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(circleInstProgram, "cameraPos2D"), view.cameraX, view.cameraY);

    glBindVertexArray(circleInstVAO[0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}
void Render::circleBatchInterop(int count, int id) {
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
    static int oldcount[max_interop] = {};
    static bool initialized[max_interop] = {};
    if (!initialized[id] || count != oldcount[id])
    {
        initInstancedCircleBuffer2d(count, id);
        initialized[id] = true;
        oldcount[id] = count;
    }
   
    if (count == 0)
        return;
    glBindBuffer(GL_ARRAY_BUFFER, circleInstDataVBO[id]);
    glUseProgram(circleInstProgram);
    glUniform2f(glGetUniformLocation(circleInstProgram, "screenSize"),
        (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(circleInstProgram, "cameraPos2D"), view.cameraX, view.cameraY);

    glBindVertexArray(circleInstVAO[id]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}
void Render::line(float x1, float y1, float x2, float y2, float r, float g, float b)
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
    vec2 n1 = PixelToNDC(x1 - view.cameraX + noRender.getScreenWidth() * 0.5f, y1 - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());
    vec2 n2 = PixelToNDC(x2 - view.cameraX + noRender.getScreenWidth() * 0.5f, y2 - view.cameraY + noRender.getScreenHeight() * 0.5f, noRender.getScreenWidth(), noRender.getScreenHeight());

    float vertices[] = {
        n1.x, n1.y, r, g, b,
        n2.x, n2.y, r, g, b};

    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glUseProgram(lineprogram);
    glUniform3f(glGetUniformLocation(lineprogram, "col"), r, g, b);
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}
void Render::lineBatch(std::vector<linepoint2d> &points)
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

    int count = (int)points.size();
    static int oldcount = 0;
    static bool firstcall = true;
    if (firstcall || count != oldcount)
    {
        initlineinstancedBuffer2d(count,0);
        firstcall = false;
        oldcount = count;
    }
    if (count < 2)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, chainVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, count * 7 * sizeof(float), points.data(), GL_STREAM_DRAW);

    glUseProgram(chainprogram);
    glUniform2f(glGetUniformLocation(chainprogram, "screenSize"),
        (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(chainprogram, "cameraPos2D"), view.cameraX, view.cameraY);

    glBindVertexArray(chainVAO[0]);
    glDrawArraysInstanced(GL_LINES, 0, 2, count);
    glBindVertexArray(0);
}
void Render::lineBatchInterop(int c,int id)
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
    if (id == 0) {
        printf(" id 0 is already assigned use 1 ");
        return;
    }
    static int oldcount[max_interop] = {};
    static bool initialized[max_interop] = {};
    if (!initialized[id] || c != oldcount[id])
    {
        initlineinstancedBuffer2d(c,id);
        initialized[id] = true;
        oldcount[id] = c;
    }
   

    glBindBuffer(GL_ARRAY_BUFFER, chainVBO[id]);
  

    glUseProgram(chainprogram);
    glUniform2f(glGetUniformLocation(chainprogram, "screenSize"),
        (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(chainprogram, "cameraPos2D"), view.cameraX, view.cameraY);

    glBindVertexArray(chainVAO[id]);
    glDrawArraysInstanced(GL_LINES, 0, 2, c);
    glBindVertexArray(0);
}

void Render::textureQuad(std::vector<quadtexture2d>& quadscreen, float positonX, float positonY, float quadWidth, float quadHeight, int pixelX, int pixelY) {


    if ((int)quadscreen.size() != pixelX * pixelY)
    {
        printf("ERROR: quadscreen size (%zu) != pixelX*pixelY (%d)\n", quadscreen.size(), pixelX * pixelY);
        return;
    }

    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }

    static bool firstcall = true;
    if (firstcall) {
        initquadfsbuffer(pixelX,pixelY,0);
        firstcall = false;
    }

    static int prevW = 0, prevH = 0;
    bool resized = (pixelX != prevW || pixelY != prevH);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, quadfsTEX[0]);

    if (resized)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pixelX, pixelY, 0, GL_RGBA, GL_FLOAT, quadscreen.data());
        prevW = pixelX; 
        prevH = pixelY;
    }
    else
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pixelX, pixelY, GL_RGBA, GL_FLOAT, quadscreen.data());
    }

    glUseProgram(quadfsprogram);
    glUniform2f(glGetUniformLocation(quadfsprogram, "uPosition"), positonX, positonY);
    glUniform2f(glGetUniformLocation(quadfsprogram, "uSize"), quadWidth, quadHeight);
    glUniform2f(glGetUniformLocation(quadfsprogram, "screenSize"),
        (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(quadfsprogram, "cameraPos2D"), view.cameraX, view.cameraY);
    glUniform1i(glGetUniformLocation(quadfsprogram, "uTex"), 0);

    glBindVertexArray(quadfsVAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
}

void Render::textureQuadInterop(float positonX, float positonY, float quadWidth, float quadHeight, int pixelX, int pixelY, int id) {


    

    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error)
        {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }

    static bool firstcall = true;
    if (firstcall) {
        initquadfsbuffer(pixelX , pixelY, id);
        firstcall = false;
    }

    static int prevW = 0, prevH = 0;
    bool resized = (pixelX != prevW || pixelY != prevH);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, quadfsTEX[id]);

    
  

    glUseProgram(quadfsprogram);
    glUniform2f(glGetUniformLocation(quadfsprogram, "uPosition"), positonX, positonY);
    glUniform2f(glGetUniformLocation(quadfsprogram, "uSize"), quadWidth, quadHeight);
    glUniform2f(glGetUniformLocation(quadfsprogram, "screenSize"),
        (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(quadfsprogram, "cameraPos2D"), view.cameraX, view.cameraY);
    glUniform1i(glGetUniformLocation(quadfsprogram, "uTex"), 0);

    glBindVertexArray(quadfsVAO[id]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

}

void Render::sprite(spriteData & sprite, float positonX, float positonY, float size) {
    if (sprite.id == 0 || sprite.width <= 0 || sprite.height <= 0)
        return;
    if (noRender.getMode() == 3) {
        static bool error = true;
        if (error) {
            printf("ERROR: use render3D class instead of render2D for 3D scene\n");
            error = false;
        }
        return;
    }

    if (quadfsVAO[0] == 0)
        initquadfsbuffer(1, 1, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.id);

    glUseProgram(quadfsprogram);
    glUniform2f(glGetUniformLocation(quadfsprogram, "uPosition"), positonX, positonY);
    glUniform2f(glGetUniformLocation(quadfsprogram, "uSize"), size, size);
    glUniform2f(glGetUniformLocation(quadfsprogram, "screenSize"),
        (float)noRender.getScreenWidth(), (float)noRender.getScreenHeight());
    glUniform2f(glGetUniformLocation(quadfsprogram, "cameraPos2D"), view.cameraX, view.cameraY);
    glUniform1i(glGetUniformLocation(quadfsprogram, "uTex"), 0);

    glBindVertexArray(quadfsVAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Render::Mesh2D() {

}

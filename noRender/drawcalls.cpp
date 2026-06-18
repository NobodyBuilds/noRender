#include "norender.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "frag.h"
#include "vertex.h"
#include "helper.h"

GLuint triangleprogram = 0;

void norender::drawtriangle(float x ,float y, float r,float g,float b,float size ) {
    float s =  size;
    Vector2D n1 = PixelToNDC(x - s , y-s, d.screenwidth, d.screeheight);
    Vector2D n2 = PixelToNDC(x , y+s, d.screenwidth, d.screeheight);
    Vector2D n3 = PixelToNDC(x+ s, y-s, d.screenwidth, d.screeheight);
    float vertices[] = {

           n1.x,n1.y,        r, g, b,
            n2.x, n2.y,        r, g, b,
            n3.x, n3.y ,       r, g, b
    };
    unsigned int  VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    triangleprogram = createProgram(trianglevert, trianglefrag);

    glUseProgram(triangleprogram);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);


}

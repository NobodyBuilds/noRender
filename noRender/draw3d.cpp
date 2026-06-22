#include "norender.h"
#include"buffers.h"
#include "glm/glm.hpp"
#include "camera.h"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>


void Render3d::drawtriangle3d(float x, float y, float z,
    float size,
    float rotX, float rotY, float rotZ,
    float r, float g, float b) {

    static bool firstcall = true;
    if (firstcall) {
        initTriangleBuffer3d();
        firstcall = false;
    }

   
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(x, y, z));
    model = glm::rotate(model, glm::radians(rotY), glm::vec3(0, 1, 0));  // yaw
    model = glm::rotate(model, glm::radians(rotX), glm::vec3(1, 0, 0));  // pitch
    model = glm::rotate(model, glm::radians(rotZ), glm::vec3(0, 0, 1));  // roll
    model = glm::scale(model, glm::vec3(size));

    glm::mat4 mvp = getProjMatrix() * getViewMatrix() * model;

    glUseProgram(tri3dProgram);
    glUniformMatrix4fv(glGetUniformLocation(tri3dProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform3f(glGetUniformLocation(tri3dProgram, "uColor"), r, g, b);
    glBindVertexArray(tri3dVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Render3d::drawQuad3d(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    float x4, float y4, float z4,
    float r, float g, float b) {

    static bool firstcall = true;
    if (firstcall) {
        initQuadBuffer3d();
        firstcall = false;
    }
    float verts[] = {
       x1,y1,z1, r,g,b,
       x3,y3,z3, r,g,b,
       x4,y4,z4, r,g,b,
       x1,y1,z1, r,g,b,
       x2,y2,z2, r,g,b,
       x4,y4,z4, r,g,b,
    };
    glm::mat4 vp = getProjMatrix() * getViewMatrix();

    glBindBuffer(GL_ARRAY_BUFFER, quad3dVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glUseProgram(quad3dProgram);
    glUniformMatrix4fv(glGetUniformLocation(quad3dProgram, "vp"), 1, GL_FALSE, glm::value_ptr(vp));
    glBindVertexArray(quad3dVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

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
    float d = size / 3.0f;
    float vert[] = {
        -d, 0, -d,
     0, 0,  d,
     d, 0, -d
    };

    
    float rX = rotX * (3.14159265f / 180.0f);
    float rY = rotY * (3.14159265f / 180.0f);
    float rZ = rotZ * (3.14159265f / 180.0f);
   
    glm::quat qx = glm::angleAxis(rX, glm::vec3(1, 0, 0));
    glm::quat qy = glm::angleAxis(rY, glm::vec3(0, 1, 0));
    glm::quat qz = glm::angleAxis(rZ, glm::vec3(0, 0, 1));

    glm::quat rot = qz * qy * qx;
    
    glBindBuffer(GL_ARRAY_BUFFER, tri3dVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vert), vert);
    glUseProgram(tri3dProgram);
    glUniform3f(glGetUniformLocation(tri3dProgram, "uPos"), x, y, z);

    glUniformMatrix4fv(glGetUniformLocation(tri3dProgram, "uproj"), 1, GL_FALSE,glm::value_ptr(getProjMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(tri3dProgram, "uview"), 1, GL_FALSE,glm::value_ptr(getViewMatrix()));
    glUniform4f(glGetUniformLocation(tri3dProgram, "rot"),rot.x,rot.y,rot.z,rot.w);
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

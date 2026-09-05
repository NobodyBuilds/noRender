#include "norender.h"
#include"buffers.h"
#include "glm/glm.hpp"
#include "camera.h"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>


void Render::triangle3D(float x, float y, float z,
    float size,
    float rotX, float rotY, 
    float r, float g, float b) {

    static bool firstcall = true;
    if (firstcall) {
        initTriangleBuffer3d();
        firstcall = false;
    }
    float d = size;
    float eq = size * 0.577f;
    float vert[] = {
        -d, 0, -eq,
     0, 0,  d* 1.155f,
     d, 0, -eq
    };

    
    float rX = rotY * (3.14159265f / 180.0f);
    float rY = rotX * (3.14159265f / 180.0f);
  
   
    glm::quat qx = glm::angleAxis(rX, glm::vec3(1, 0, 0));
    glm::quat qy = glm::angleAxis(rY, glm::vec3(0, 1, 0));
  

    glm::quat rot = qy * qx;
    
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

void Render::triangleBatch3D(std::vector<triangle3d> &instances) {
    static bool firstcall = true;
    if (firstcall) {
        inittriangle3dinstbuffer((int)instances.size(), 0);
        firstcall = false;
    }
    int count = (int)instances.size();
    if (count < 1)
        return;
    glBindBuffer(GL_ARRAY_BUFFER, triangle3dDataVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle3d) * count, instances.data(), GL_STREAM_DRAW);
    glm::mat4 vp = getProjMatrix() * getViewMatrix();
    glUseProgram(triangle3dProgram[0]);
    glUniformMatrix4fv(glGetUniformLocation(triangle3dProgram[0], "vp"), 1, GL_FALSE, glm::value_ptr(vp));
    glBindVertexArray(triangle3dVAO[0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);
    glBindVertexArray(0);
}

void Render::quad3D(float x1, float y1, float z1,
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

void Render::rawTriangles3D() {}

void Render::drawMesh(MeshData& mesh, float x, float y, float z, float size,
	float rotX, float rotY,
	float r, float g, float b,
	bool useModelColor, bool showNormal)
{
	glm::mat4 vp = getProjMatrix() * getViewMatrix();

	float rx = rotX * (3.14159265f / 180.0f);
	float ry = rotY * (3.14159265f / 180.0f);

	glUseProgram(meshProgram);
	glUniformMatrix4fv(glGetUniformLocation(meshProgram, "vp"), 1, GL_FALSE, glm::value_ptr(vp));
	glUniform3f(glGetUniformLocation(meshProgram, "uPos"), x, y, z);
	glUniform2f(glGetUniformLocation(meshProgram, "uRot"), rx, ry);
	glUniform1f(glGetUniformLocation(meshProgram, "uScale"), size);
	glUniform3f(glGetUniformLocation(meshProgram, "uColor"), r, g, b);
	glUniform1i(glGetUniformLocation(meshProgram, "uUseModelColor"), useModelColor ? 1 : 0);
	glUniform1i(glGetUniformLocation(meshProgram, "uShowNormal"), showNormal ? 1 : 0);
	glUniform3f(glGetUniformLocation(meshProgram, "uCameraPos"), camera.position.x, camera.position.y, camera.position.z);

	glBindVertexArray(mesh.vao);
	if (mesh.hasTransparency && useModelColor) {
		GLint passLocation = glGetUniformLocation(meshProgram, "uAlphaPass");

		glUniform1i(passLocation, 0);
		glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);

		GLboolean wasBlendEnabled = glIsEnabled(GL_BLEND);
		GLboolean previousDepthMask = GL_TRUE;
		glGetBooleanv(GL_DEPTH_WRITEMASK, &previousDepthMask);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		glUniform1i(passLocation, 1);
		glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);

		glDepthMask(previousDepthMask);
		if (!wasBlendEnabled) {
			glDisable(GL_BLEND);
		}
	}
	else {
		glUniform1i(glGetUniformLocation(meshProgram, "uAlphaPass"), 2);
		glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

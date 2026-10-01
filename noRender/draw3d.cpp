#include "norender.h"
#include"buffers.h"
#include "glm/glm.hpp"
#include "camera.h"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>


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
    int count = (int)instances.size();
    static int oldcount = 0;
    static bool firstcall = true;
    if (firstcall || count != oldcount) {
        inittriangle3dinstbuffer(count, 0);
        firstcall = false;
        oldcount = count;
    }
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
       x2,y2,z2, r,g,b,
       x3,y3,z4, r,g,b,
       x1,y1,z1, r,g,b,
       x3,y3,z3, r,g,b,
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

void Render::quad3DBatch(std::vector<quadVertex3d>& instances) {
    int count = (int)instances.size();
    static int oldcount = 0;
    static bool firstcall = true;
    if (firstcall || count != oldcount) {
        initQuad3dInstBuffer(count, 0);
        firstcall = false;
        oldcount = count;
    }
    if (count < 1)
        return;
    glBindBuffer(GL_ARRAY_BUFFER, quad3dInstDataVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertex3d) * count, instances.data(), GL_STREAM_DRAW);
    glm::mat4 vp = getProjMatrix() * getViewMatrix();
    glUseProgram(quad3dInstProgram);
    glUniformMatrix4fv(glGetUniformLocation(quad3dInstProgram, "vp"), 1, GL_FALSE, glm::value_ptr(vp));
    glBindVertexArray(quad3dInstVAO[0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
    glBindVertexArray(0);
}

void Render::quad3DBatchInterop3D(int count, int id) {
    static int oldcount[max_interop] = {};
    static bool initialized[max_interop] = {};
    if (!initialized[id] || count != oldcount[id]) {
        initQuad3dInstBuffer(count, id);
        initialized[id] = true;
        oldcount[id] = count;
    }
    if (count == 0)
        return;
    glm::mat4 vp = getProjMatrix() * getViewMatrix();
    glUseProgram(quad3dInstProgram);
    glUniformMatrix4fv(glGetUniformLocation(quad3dInstProgram, "vp"), 1, GL_FALSE, glm::value_ptr(vp));
    glBindVertexArray(quad3dInstVAO[id]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
    glBindVertexArray(0);
}

void Render::rawTriangle3D(rawTriangles3D& tri) {
    static bool firstcall = true;
    if (firstcall) {
        initRawTriangleBuffer3d();
        firstcall = false;
    }
    float verts[] = {
        tri.vertex1.x, tri.vertex1.y, tri.vertex1.z, tri.r, tri.g, tri.b,
        tri.vertex2.x, tri.vertex2.y, tri.vertex2.z, tri.r, tri.g, tri.b,
        tri.vertex3.x, tri.vertex3.y, tri.vertex3.z, tri.r, tri.g, tri.b,
    };

    glBindBuffer(GL_ARRAY_BUFFER, rawTri3dVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glm::mat4 vp = getProjMatrix() * getViewMatrix();
    glUseProgram(rawTri3dProgram);
    glUniformMatrix4fv(glGetUniformLocation(rawTri3dProgram, "vp"), 1, GL_FALSE, glm::value_ptr(vp));
    glBindVertexArray(rawTri3dVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Render::rawTriangleBatch3D(std::vector<rawTriangles3D>& tris) {
    int count = (int)tris.size();
    static int oldcount = 0;
    static bool firstcall = true;
    if (firstcall || count != oldcount) {
        initRawTriangleBatchBuffer3d(count, 0);
        firstcall = false;
        oldcount = count;
    }
    if (count < 1)
        return;

    // Build unique vertex set and index buffer for indexed rendering
    struct VertKey {
        float x, y, z, r, g, b;
        bool operator==(const VertKey& o) const {
            return x == o.x && y == o.y && z == o.z && r == o.r && g == o.g && b == o.b;
        }
    };
    struct VertHash {
        size_t operator()(const VertKey& v) const {
            size_t h = 0;
            auto hf = std::hash<float>{};
            h ^= hf(v.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hf(v.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hf(v.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hf(v.r) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hf(v.g) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hf(v.b) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::unordered_map<VertKey, unsigned int, VertHash> vertMap;
    vertices.reserve(count * 3 * 6);
    indices.reserve(count * 3);

    auto addVert = [&](float x, float y, float z, float r, float g, float b) {
        VertKey key = { x, y, z, r, g, b };
        auto it = vertMap.find(key);
        if (it != vertMap.end()) {
            indices.push_back(it->second);
        } else {
            unsigned int idx = (unsigned int)(vertices.size() / 6);
            vertMap[key] = idx;
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
            vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
            indices.push_back(idx);
        }
    };

    for (int i = 0; i < count; i++) {
        addVert(tris[i].vertex1.x, tris[i].vertex1.y, tris[i].vertex1.z, tris[i].r, tris[i].g, tris[i].b);
        addVert(tris[i].vertex2.x, tris[i].vertex2.y, tris[i].vertex2.z, tris[i].r, tris[i].g, tris[i].b);
        addVert(tris[i].vertex3.x, tris[i].vertex3.y, tris[i].vertex3.z, tris[i].r, tris[i].g, tris[i].b);
    }

    // Upload unique vertices
    glBindBuffer(GL_ARRAY_BUFFER, rawTri3dBatchVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);

    // Bind VAO (restores EBO association) and upload indices
    glBindVertexArray(rawTri3dBatchVAO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STREAM_DRAW);

    glm::mat4 vp = getProjMatrix() * getViewMatrix();
    glUseProgram(rawTri3dBatchProgram);
    glUniformMatrix4fv(glGetUniformLocation(rawTri3dBatchProgram, "vp"), 1, GL_FALSE, glm::value_ptr(vp));
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Render::rawTriangleBatchInterop3D(int count, int id) {
    static int oldcount[max_interop] = {};
    static bool initialized[max_interop] = {};
    if (!initialized[id] || count != oldcount[id]) {
        initRawTriangleBatchBuffer3d(count, id);
        initialized[id] = true;
        oldcount[id] = count;
    }
    if (count == 0)
        return;
    glm::mat4 vp = getProjMatrix() * getViewMatrix();
    glUseProgram(rawTri3dBatchProgram);
    glUniformMatrix4fv(glGetUniformLocation(rawTri3dBatchProgram, "vp"), 1, GL_FALSE, glm::value_ptr(vp));
    glBindVertexArray(rawTri3dBatchVAO[id]);
    glDrawArrays(GL_TRIANGLES, 0, count * 3);
    glBindVertexArray(0);
}

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

void Render::textureQuad3D(std::vector<quadtexture2d>& quadscreen, float positonX, float positonY, float positonZ, float quadWidth, float quadHeight, int pixelX, int pixelY, float rotX, float rotY) {
    glEnable(GL_BLEND);
    if ((int)quadscreen.size() != pixelX * pixelY) {
        printf("ERROR: quadscreen size (%zu) != pixelX*pixelY (%d)\n", quadscreen.size(), pixelX * pixelY);
        return;
    }

    if (noRender.getMode() == 2) {
        static bool error = true;
        if (error) {
            printf("ERROR: use render2D class instead of render3D for 2D scene\n");
            error = false;
        }
        return;
    }

    static bool firstcall = true;
    if (firstcall) {
        initquadfsbuffer(pixelX,pixelY,0);
        if (quadfsprogram3D == 0) {
            quadfsprogram3D = createProgram(quadfs3DVert, quadfsfrag);
        }
        firstcall = false;
    }

    static int prevW = 0, prevH = 0;
    bool resized = (pixelX != prevW || pixelY != prevH);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, quadfsTEX[0]);

    if (resized) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pixelX, pixelY, 0, GL_RGBA, GL_FLOAT, quadscreen.data());
        prevW = pixelX; 
        prevH = pixelY;
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pixelX, pixelY, GL_RGBA, GL_FLOAT, quadscreen.data());
    }

    glUseProgram(quadfsprogram3D);
    glUniform3f(glGetUniformLocation(quadfsprogram3D, "uPosition"), positonX, positonY, positonZ);
    glUniform2f(glGetUniformLocation(quadfsprogram3D, "uSize"), quadWidth, quadHeight);
    
    float rX = rotX * (3.14159265f / 180.0f);
    float rY = rotY * (3.14159265f / 180.0f);
    glUniform2f(glGetUniformLocation(quadfsprogram3D, "uRot"), rX, rY);
    
    glUniformMatrix4fv(glGetUniformLocation(quadfsprogram3D, "vp"), 1, GL_FALSE, glm::value_ptr(getProjMatrix() * getViewMatrix()));
    glUniform1i(glGetUniformLocation(quadfsprogram3D, "uTex"), 0);

    glBindVertexArray(quadfsVAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void Render::textureQuadInterop3D(float positonX, float positonY, float positonZ, float quadWidth, float quadHeight, int pixelX, int pixelY, int id, float rotX, float rotY) {
    if (noRender.getMode() == 2) {
        static bool error = true;
        if (error) {
            printf("ERROR: use render2D class instead of render3D for 2D scene\n");
            error = false;
        }
        return;
    }

    static bool firstcall = true;
    if (firstcall) {
        initquadfsbuffer(pixelX, pixelY, id);
        if (quadfsprogram3D == 0) {
            quadfsprogram3D = createProgram(quadfs3DVert, quadfsfrag);
        }
        firstcall = false;
    }

    static int prevW = 0, prevH = 0;
    bool resized = (pixelX != prevW || pixelY != prevH);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, quadfsTEX[id]);

    if (resized) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pixelX, pixelY, 0, GL_RGBA, GL_FLOAT, nullptr);
        prevW = pixelX;
        prevH = pixelY;
    }

    glUseProgram(quadfsprogram3D);
    glUniform3f(glGetUniformLocation(quadfsprogram3D, "uPosition"), positonX, positonY, positonZ);
    glUniform2f(glGetUniformLocation(quadfsprogram3D, "uSize"), quadWidth, quadHeight);

    float rX = rotX * (3.14159265f / 180.0f);
    float rY = rotY * (3.14159265f / 180.0f);
    glUniform2f(glGetUniformLocation(quadfsprogram3D, "uRot"), rX, rY);

    glUniformMatrix4fv(glGetUniformLocation(quadfsprogram3D, "vp"), 1, GL_FALSE, glm::value_ptr(getProjMatrix() * getViewMatrix()));
    glUniform1i(glGetUniformLocation(quadfsprogram3D, "uTex"), 0);

    glBindVertexArray(quadfsVAO[id]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Render::sprite3D(spriteData& sprite, float positonX, float positonY, float positonZ, float size, float rotX, float rotY) {
    if (sprite.id == 0 || sprite.width <= 0 || sprite.height <= 0)
        return;
    if (noRender.getMode() == 2) {
        static bool error = true;
        if (error) {
            printf("ERROR: use render2D class instead of render3D for 2D scene\n");
            error = false;
        }
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (quadfsVAO[0] == 0)
        initquadfsbuffer(1, 1, 0);
    if (quadfsprogram3D == 0)
        quadfsprogram3D = createProgram(quadfs3DVert, quadfsfrag);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.id);
    glUseProgram(quadfsprogram3D);
    glUniform3f(glGetUniformLocation(quadfsprogram3D, "uPosition"), positonX, positonY, positonZ);
    glUniform2f(glGetUniformLocation(quadfsprogram3D, "uSize"), size, size);

    float rX = rotX * (3.14159265f / 180.0f);
    float rY = rotY * (3.14159265f / 180.0f);
    glUniform2f(glGetUniformLocation(quadfsprogram3D, "uRot"), rX, rY);
    glUniformMatrix4fv(glGetUniformLocation(quadfsprogram3D, "vp"), 1, GL_FALSE,
        glm::value_ptr(getProjMatrix() * getViewMatrix()));
    glUniform1i(glGetUniformLocation(quadfsprogram3D, "uTex"), 0);

    glBindVertexArray(quadfsVAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderdata.h"
#include "vertex.h"
#include "frag.h"
#include "helper.h"

// 2d
inline void initTriangleBuffer2d()
{
	if (triVAO != 0)
		return;
	glGenBuffers(1, &triVBO);													   // buffer id
	glGenVertexArrays(1, &triVAO);												   // vertex array which holds data
	glBindVertexArray(triVAO);													   // binding vertexarray
	glBindBuffer(GL_ARRAY_BUFFER, triVBO);										   // binnding buffer
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), nullptr, GL_STREAM_DRAW); // allocating buffer data ,nullptr =empty data

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0); // layout 0 data transfer

	glBindVertexArray(0); // idk what this

	triangleprogram = createProgram(trianglevert, trianglefrag);
}

inline void initCircleBuffer2d()
{
	if (cirVAO != 0)
		return;
	glGenVertexArrays(1, &cirVAO);
	glGenBuffers(1, &cirVBO);

	glBindVertexArray(cirVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cirVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 7 * sizeof(float), nullptr, GL_STREAM_DRAW); // reserve space, no data yet

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	circleprogram = createProgram(circlevert, circlefrag);
}

inline void initQuadBuffer2d()
{
	if (quadVAO != 0)
		return;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), nullptr, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	quadprogram = createProgram(trianglevert, trianglefrag);
}

//instanced

inline void initInstancedTriangleBuffer2d(int count,int id)
{
	static int prevcount[max_interop] = { 0 };
	if (triInstVAO != 0 && prevcount[id] >= count)
		return;
	if (triInstVAO[id] == 0) {
		glGenVertexArrays(1, &triInstVAO[id]);
		glGenBuffers(1, &triInstBaseVBO[id]);
		glGenBuffers(1, &triInstDataVBO[id]);
	}
	float tribaseVerts[] = {
	 -1.0f, -0.577f,   // bottom left
	 0.0f,  1.155f,   // apex
	 1.0f, -0.577f };

	glBindVertexArray(triInstVAO[id]);

	glBindBuffer(GL_ARRAY_BUFFER, triInstBaseVBO[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tribaseVerts), tribaseVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);
	if (prevcount[id] < count) {
		glBindBuffer(GL_ARRAY_BUFFER, triInstDataVBO[id]);
		glBufferData(GL_ARRAY_BUFFER, count * 7 * sizeof(float), nullptr, GL_STATIC_DRAW);
		prevcount[id] = count;
	}
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(2 * sizeof(float))); // size
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(3 * sizeof(float))); // color
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);



	glBindVertexArray(0);


	if (triInstProgram == 0) {

	triInstProgram = createProgram(triInstancedVert, trianglefrag);
	}
}



inline void initInstancedCircleBuffer2d(int count,int id)
{
	static int prevcount[max_interop] = { 0 };
	if (circleInstVAO[id] != 0 && prevcount[id] >= count)
		return;
	if (circleInstVAO[id] == 0) {
		glGenVertexArrays(1, &circleInstVAO[id]);
		glGenBuffers(1, &circleInstBaseVBO[id]);
		glGenBuffers(1, &circleInstDataVBO[id]);
	}
	float circlebaseVerts[] = {
		-1.0f, -1.0f,
		3.0f, -1.0f,
		-1.0f, 3.0f};

	glBindVertexArray(circleInstVAO[id]);

	glBindBuffer(GL_ARRAY_BUFFER, circleInstBaseVBO[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circlebaseVerts), circlebaseVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);
	if (prevcount[id] < count) {
		glBindBuffer(GL_ARRAY_BUFFER, circleInstDataVBO[id]);
		glBufferData(GL_ARRAY_BUFFER, count * 6 * sizeof(float), circlebaseVerts, GL_STATIC_DRAW);
		prevcount[id] = count;
	}
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0); // pos
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(2 * sizeof(float))); // size
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float))); // color
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
	if (circleInstProgram == 0) {

	circleInstProgram = createProgram(circleInstancedVert, circlefrag);
	}
}

inline void initInstancedQuadBuffer2d(int count,int id)


{
	static int prevcount[max_interop] = { 0 };
	if (quadInstVAO[id] != 0 && prevcount[id] >= count)
		return;

	
	if (quadInstVAO[id] == 0) {
		glGenVertexArrays(1, &quadInstVAO[id]);
		glGenBuffers(1, &quadInstBaseVBO[id]);
		glGenBuffers(1, &quadInstDataVBO[id]);
		glGenBuffers(1, &quadInstEbo[id]);
	}
	float quadbaseVerts[] = {
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f

	};
	float indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glBindVertexArray(quadInstVAO[id]);
	
	glBindBuffer(GL_ARRAY_BUFFER, quadInstBaseVBO[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadbaseVerts), quadbaseVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);
	if (prevcount[id]<count) {
		glBindBuffer(GL_ARRAY_BUFFER, quadInstDataVBO[id]);
		glBufferData(GL_ARRAY_BUFFER, count * 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
		prevcount[id] = count;
	}
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(2 * sizeof(float))); // size
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(4 * sizeof(float))); // color
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(7 * sizeof(float))); // rotation val
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadInstEbo[id]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	if (quadInstProgram == 0) {
		quadInstProgram = createProgram(quadInstancedVert, trianglefrag);
	}
}

inline void initLineBuffer2d()
{
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * 5 * sizeof(float), nullptr, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	lineprogram = createProgram(trianglevert, trianglefrag);
}

inline void initlineinstancedBuffer2d(int count,int id)
{
	static int prevcount[max_interop] = { 0 };
	if (chainVAO[id] != 0 && prevcount[id] >= count)
		return;
	if (chainVAO[id] == 0) {
		glGenVertexArrays(1, &chainVAO[id]);
		glGenBuffers(1, &chainVBO[id]);
		glGenBuffers(1, &chainbaseVBO[id]);
	}

	float lineBaseVerts[] = { 0.0f, 1.0f };

	glBindVertexArray(chainVAO[id]);

	glBindBuffer(GL_ARRAY_BUFFER, chainbaseVBO[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineBaseVerts), lineBaseVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);
	if (prevcount[id] < count) {
		glBindBuffer(GL_ARRAY_BUFFER, chainVBO[id]);
		glBufferData(GL_ARRAY_BUFFER, count * 7 * sizeof(float), nullptr, GL_STREAM_DRAW);
		prevcount[id] = count;
	}
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0); // ox,oy
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float))); // dx,dy
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float))); // color
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
	if (chainprogram == 0) {
		chainprogram = createProgram(chainVert, trianglefrag);
	}
}

inline void initquadfsbuffer(int width,int height, int id) {
	static int prevbuffer[max_interop] = { 0 };
	if (quadfsVAO[id] != 0 && prevbuffer[id] >= width*height) {
		return;
	}
	if (quadfsVAO[id] == 0) {
		glGenVertexArrays(1, &quadfsVAO[id]); 
		glGenBuffers(1, &quadfsVBO[id]);
	}
	float quadFSVerts[] = {
		       
		-1.0f,  1.0f,   0.0f, 1.0f,
		-1.0f, -1.0f,   0.0f, 0.0f,
		 1.0f, -1.0f,   1.0f, 0.0f,

		-1.0f,  1.0f,   0.0f, 1.0f,
		 1.0f,  1.0f,   1.0f, 1.0f,
		 1.0f, -1.0f,   1.0f, 0.0f
	};

	glBindVertexArray(quadfsVAO[id]);
	glBindBuffer(GL_ARRAY_BUFFER, quadfsVBO[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadFSVerts), quadFSVerts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	if (quadfsTEX[id] == 0) {
		glGenTextures(1, &quadfsTEX[id]);
	}
	glBindTexture(GL_TEXTURE_2D, quadfsTEX[id]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_LINEAR if want smooth stretch instead of chunky pixels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	quadfsprogram = createProgram(quadfsVert, quadfsfrag);
}
// 3d

inline void initTriangleBuffer3d()
{
	if (tri3dVAO != 0)
		return;
	float unitTri[] = {
		-0.5f,
		-0.5f,
		0.f,
		0.0f,
		0.5f,
		0.f,
		0.5f,
		-0.5f,
		0.f,
	};

	glGenVertexArrays(1, &tri3dVAO);
	glGenBuffers(1, &tri3dVBO);
	glBindVertexArray(tri3dVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tri3dVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unitTri), unitTri, GL_STATIC_DRAW); // never changes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	tri3dProgram = createProgram(triangle3dvert, triangle3dfrag);
}

inline void inittriangle3dinstbuffer(int count, int id) {
	static int prevcount[max_interop] = { 0 };
	if(triangle3dVAO[id] != 0 && prevcount[id] >= count) {
		return;
	}
	if(triangle3dVAO[id] == 0) {
		glGenVertexArrays(1, &triangle3dVAO[id]);
		glGenBuffers(1, &triangle3dbaseVBO[id]);
		glGenBuffers(1, &triangle3dDataVBO[id]);
	}
	float tribaseverts[] = {
	-1.0f,0.0f, -0.577f,   // bottom left
	 0.0f,0.0f,  1.155f,   // apex
	 1.0f,0.0f, -0.577f 
	};

	glBindVertexArray(triangle3dVAO[id]);
	glBindBuffer(GL_ARRAY_BUFFER, triangle3dbaseVBO[id]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tribaseverts) , tribaseverts, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);
	if (prevcount[id] < count) {
		glBindBuffer(GL_ARRAY_BUFFER, triangle3dDataVBO[id]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle3d) * count, nullptr, GL_STATIC_DRAW);
		prevcount[id] = count;
	}
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(triangle3d), (void*)0);// position
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(triangle3d), (void*)(3 * sizeof(float)));//  size
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(triangle3d), (void*)(4 * sizeof(float)));//  rotation
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);	
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(triangle3d), (void*)(6 * sizeof(float)));// color
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);
	glBindVertexArray(0);
	triangle3dProgram[id] = createProgram(tri3dinstvert, triangle3dfrag);
	prevcount[id] = count;
}

inline void initQuadBuffer3d()
{
	if (quad3dVAO != 0)
		return;
	glGenVertexArrays(1, &quad3dVAO);
	glGenBuffers(1, &quad3dVBO);

	glBindVertexArray(quad3dVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quad3dVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * 6 * sizeof(float), nullptr, GL_STREAM_DRAW); // reserve space, no data yet

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	quad3dProgram = createProgram(triangle3dvert, triangle3dfrag);
}

// raw triangle 3D (single)
inline void initRawTriangleBuffer3d()
{
	if (rawTri3dVAO != 0)
		return;
	glGenVertexArrays(1, &rawTri3dVAO);
	glGenBuffers(1, &rawTri3dVBO);
	glBindVertexArray(rawTri3dVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rawTri3dVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), nullptr, GL_STREAM_DRAW); // 3 verts × (3 pos + 3 color)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // color
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	rawTri3dProgram = createProgram(rawTri3dVert, triangle3dfrag);
}

// raw triangle 3D (batch/interop)
inline void initRawTriangleBatchBuffer3d(int count, int id)
{
	static int prevcount[max_interop] = { 0 };
	if (rawTri3dBatchVAO[id] != 0 && prevcount[id] >= count)
		return;
	if (rawTri3dBatchVAO[id] == 0) {
		glGenVertexArrays(1, &rawTri3dBatchVAO[id]);
		glGenBuffers(1, &rawTri3dBatchVBO[id]);
		glGenBuffers(1, &rawTri3dBatchEBO[id]);
	}

	glBindVertexArray(rawTri3dBatchVAO[id]);

	if (prevcount[id] < count) {
		glBindBuffer(GL_ARRAY_BUFFER, rawTri3dBatchVBO[id]);
		glBufferData(GL_ARRAY_BUFFER, count * 3 * 6 * sizeof(float), nullptr, GL_STREAM_DRAW); // count tris × 3 verts × 6 floats
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rawTri3dBatchEBO[id]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 3 * sizeof(unsigned int), nullptr, GL_STREAM_DRAW); // count tris × 3 indices
		prevcount[id] = count;
	}

	glBindBuffer(GL_ARRAY_BUFFER, rawTri3dBatchVBO[id]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // color
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rawTri3dBatchEBO[id]);

	glBindVertexArray(0);
	if (rawTri3dBatchProgram == 0) {
		rawTri3dBatchProgram = createProgram(rawTri3dVert, triangle3dfrag);
	}
}

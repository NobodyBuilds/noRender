#pragma once
#include <GLFW/glfw3.h>


inline constexpr int max_interop = 10;//tune this for more interops vbo ids
//2d
	//triangle
	inline unsigned int triVBO = 0;
	inline unsigned int triVAO = 0;
	inline unsigned int triangleprogram = 0;
	//circle
	inline unsigned int cirVBO = 0;
	inline unsigned int cirVAO = 0;
	inline unsigned int circleprogram = 0;
	//quad
	inline unsigned int quadVBO = 0;
	inline unsigned int quadVAO = 0;
	inline unsigned int quadprogram = 0;
	
	//instanced
	//triangle
	inline unsigned int triInstVAO[max_interop] = { 0 };
	inline unsigned int triInstBaseVBO[max_interop] = { 0 }; 
	inline unsigned int triInstDataVBO[max_interop] = { 0 };
	inline unsigned int triInstProgram = 0;
	//circle
	inline unsigned int circleInstVAO[max_interop] = {0};
	inline unsigned int circleInstBaseVBO[max_interop] = {0}; 
	inline unsigned int circleInstDataVBO[max_interop] = {0};
	inline unsigned int circleInstProgram = 0;
	//quad
	inline unsigned int quadInstVAO[max_interop] = { 0 };
	inline unsigned int quadInstBaseVBO[max_interop] = { 0 }; 
	inline unsigned int quadInstDataVBO[max_interop] = { 0 };
	inline unsigned int quadInstEbo[max_interop] = { 0 };
	inline unsigned int quadInstProgram = 0;
	//lines
	inline unsigned int lineVAO = 0, lineVBO = 0, lineprogram = 0;
	inline unsigned int chainVAO[max_interop] = { 0 }, chainVBO[max_interop] = { 0 }, chainprogram = 0 ;
	inline unsigned int chainbaseVAO[max_interop] = { 0 }, chainbaseVBO[max_interop] = { 0 };

	//texture renders
	inline unsigned int quadfsVAO[max_interop] = { 0 };
	inline unsigned int quadfsVBO[max_interop] = { 0 };
	inline unsigned int quadfsTEX[max_interop] = { 0 };
	inline unsigned int quadfsprogram = 0;


	//3d
	//triangle
	inline  GLuint tri3dVAO, tri3dVBO, tri3dProgram;
	inline  GLuint quad3dVAO, quad3dVBO, quad3dProgram;

	inline unsigned int triangle3dVAO[max_interop] = { 0 }, triangle3dbaseVBO[max_interop] = { 0 }, triangle3dProgram[max_interop] = { 0 }, triangle3dDataVBO[max_interop] = { 0 };

	// raw triangle 3D (single)
	inline GLuint rawTri3dVAO = 0, rawTri3dVBO = 0, rawTri3dProgram = 0;
	// raw triangle 3D (batch/interop)
	inline unsigned int rawTri3dBatchVAO[max_interop] = { 0 };
	inline unsigned int rawTri3dBatchVBO[max_interop] = { 0 };
	inline unsigned int rawTri3dBatchEBO[max_interop] = { 0 };
	inline unsigned int rawTri3dBatchProgram = 0;

	// mesh
	inline unsigned int meshProgram = 0;
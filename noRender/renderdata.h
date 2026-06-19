#pragma once
#include <GLFW/glfw3.h>



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
	inline unsigned int triInstVAO = 0;
	inline unsigned int triInstBaseVBO = 0; 
	inline unsigned int triInstDataVBO = 0;  
	inline unsigned int triInstProgram = 0;
	//circle
	inline unsigned int circleInstVAO = 0;
	inline unsigned int circleInstBaseVBO = 0; 
	inline unsigned int circleInstDataVBO = 0;  
	inline unsigned int circleInstProgram = 0;
	//quad
	inline unsigned int quadInstVAO = 0;
	inline unsigned int quadInstBaseVBO = 0; 
	inline unsigned int quadInstDataVBO = 0;  
	inline unsigned int quadInstProgram = 0;
	//lines
	inline unsigned int lineVAO = 0, lineVBO = 0, lineprogram = 0;
	inline unsigned int chainVAO = 0, chainVBO = 0, chainprogram = 0;

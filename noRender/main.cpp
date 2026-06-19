#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "norender.h"
#include "helper.h"
#include "vertex.h"
#include"frag.h"
#include"renderdata.h"

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) {
		printf("width or height cant be 0");
		return;
	}
	glViewport(0, 0, width, height);
};


int norender::createWindow(int width, int height, const char* Windowname, int vsync) {// vsync is 1 for on, 0 for off

	if (!glfwInit()) {
		printf("glfw init failed");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(width, height, Windowname, nullptr, nullptr);

	if (!window) {
		printf("window creation failed");
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(vsync);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to init GLAD\n";
		return -1;
	}
	d.screenwidth = width;
	d.screeheight = height;
	return 0;
};

void norender::closeWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();

};

bool norender::WindowOpen() {
	return !glfwWindowShouldClose(window);
};

void norender::processinputs() {
	glfwPollEvents();
};
void norender::swapbuffers() {
	glfwSwapBuffers(window);
}

void norender::clearscreen() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}


void norender::init() {

	//triangle
	if (triVAO != 0) return;
	glGenVertexArrays(1, &triVAO);
	glGenBuffers(1, &triVBO);

	glBindVertexArray(triVAO);
	glBindBuffer(GL_ARRAY_BUFFER, triVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(float), nullptr, GL_STREAM_DRAW); // reserve space, no data yet

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	triangleprogram = createProgram(trianglevert, trianglefrag);
	//circle
	if (cirVAO != 0) return;
	glGenVertexArrays(1, &cirVAO);
	glGenBuffers(1, &cirVBO);

	glBindVertexArray(cirVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cirVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 7 * sizeof(float), nullptr, GL_STREAM_DRAW); // reserve space, no data yet

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	circleprogram = createProgram(circlevert, circlefrag);

	//quad
	if (quadVAO != 0) return;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * 5 * sizeof(float), nullptr, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	quadprogram = createProgram(trianglevert, trianglefrag);

	//instanced
	//triangles
	glGenVertexArrays(1, &triInstVAO);
	glGenBuffers(1, &triInstBaseVBO);
	glGenBuffers(1, &triInstDataVBO);


	float tribaseVerts[] = {
		-1.0f, -1.0f,
		 0.0f,  1.0f,
		 1.0f, -1.0f
	};

	glBindVertexArray(triInstVAO);


	glBindBuffer(GL_ARRAY_BUFFER, triInstBaseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tribaseVerts), tribaseVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, triInstDataVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));// size
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));// color
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);

	triInstProgram = createProgram(triInstancedVert, trianglefrag);

	//circle
	glGenVertexArrays(1, &circleInstVAO);
	glGenBuffers(1, &circleInstBaseVBO);
	glGenBuffers(1, &circleInstDataVBO);


	float circlebaseVerts[] = {
		-1.0f, -1.0f,
		 3.0f,  -1.0f,
		 -1.0f, 3.0f
	};

	glBindVertexArray(circleInstVAO);


	glBindBuffer(GL_ARRAY_BUFFER, circleInstBaseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circlebaseVerts), circlebaseVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, circleInstDataVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  //pos                
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));// size
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));// color
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);

	circleInstProgram = createProgram(circleInstancedVert, circlefrag);

	//quad
	glGenVertexArrays(1, &quadInstVAO);
	glGenBuffers(1, &quadInstBaseVBO);
	glGenBuffers(1, &quadInstDataVBO);


	float quadbaseVerts[] = {
		-1.0f,1.0f,
		-1.0f,-1.0f,
		1.0f,-1.0f,
		-1.0f,1.0f,
		1.0f,1.0f,
		1.0f,-1.0f


	};

	glBindVertexArray(quadInstVAO);


	glBindBuffer(GL_ARRAY_BUFFER, quadInstBaseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadbaseVerts), quadbaseVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, quadInstDataVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));// size
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));// color
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);

	quadInstProgram = createProgram(quadInstancedVert, trianglefrag);

	//lines

	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * 5 * sizeof(float), nullptr, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	lineprogram = createProgram(trianglevert, trianglefrag);

	glGenVertexArrays(1, &chainVAO);
	glGenBuffers(1, &chainVBO);
	glBindVertexArray(chainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, chainVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	chainprogram = createProgram(chainVert, trianglefrag);
}
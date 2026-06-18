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

	//rectangle
	if (rectVAO != 0) return;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);

	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);

									
	glBufferData(GL_ARRAY_BUFFER, 6 * 5 * sizeof(float), nullptr, GL_STREAM_DRAW); 

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	rectprogram = createProgram(trianglevert, trianglefrag);

}

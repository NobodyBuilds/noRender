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
#include "buffers.h"

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
	norender::screenwidth = width;
	norender::screenheight = height;
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

void norender::clearscreen(float r,float g ,float b) {
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}



void norender::init() {


	
}


void norender::setup2d() {
	norender::mode = 2;
}
void norender::setup3d() {
	norender::mode = 3;
	
}
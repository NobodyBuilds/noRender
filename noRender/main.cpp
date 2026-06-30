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
#include "camera.h"







 GLFWwindow* window;



 GLFWwindow* norender::getwindowid() {
	 return window;
 }


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	if ((float)width == 0 || (float)height == 0) {
		printf("width or height cant be 0");
		return;
	}
	noRender.screensize((float)width, (float)height);
	glViewport(0, 0, width, height);
	if (noRender.getMode() == 3) {
		view.cx = (float)width * 0.5f;
		view.cz = (float)height * 0.5f;
		view.height = (float)height;
		view.aspect = (float)width / (float)height;
	};

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

	view.cx = width * 0.5f;
	view.cz = height * 0.5f;
	view.height = (float)height;
	view.aspect = (float)width / (float)height;
	view.width();
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
	if (noRender.getMode() == 3)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT);
}



void norender::init() {


	
}


void norender::setup2d() {
	norender::mode = 2;
	
}
void norender::setup3d() {
	norender::mode = 3;
	glEnable(GL_DEPTH_TEST);
	
}
void norender::setupCamera() {
	if (noRender.getMode() == 2) {
		static bool firstcall = true;
		if (firstcall) {
			printf("ERROR: camera is not available for 2d scene\n");
			firstcall = false;
		}
		return;
	}
	updateCameraVectors(camera);
	cameraLookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glfwSetScrollCallback(window, scrollCallback);
}


void norender::updateCamera() {
	if (blockedInput) return;
	updateCameraMovement(window);
}
float norender::camposx() {
	return camera.position.x;
}
float norender::camposy() {
	return camera.position.y;
}
float norender::camposz() {
	return camera.position.z;

}
void norender::screensize(float w ,float h) {
	norender::screenwidth = (int)w;
	norender::screenheight = (int)h;
}

void norender::setInputBlocked(bool blocked) {
	blockedInput = blocked;
}

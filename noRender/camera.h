#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "norender.h"
#include<GLFW/glfw3.h>

struct View {

	float cx ;
	
	float cz ;
	float height =1.0f;
	float aspect =5.0f;
	float zoom = 1.f;
	float width() const { return height * aspect; };

};
inline View view;

struct Camera {
	glm::vec3 position = glm::vec3(0.0f, 5.f, 10.f);
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	float yaw = -45.0f;
	float pitch = -15.0f;
	float fov = 70.0f;
};
inline Camera camera;



inline void updateCameraVectors(Camera& cam)
{
	float yawRad = glm::radians(cam.yaw);
	float pitchRad = glm::radians(cam.pitch);
	
	cam.forward = glm::normalize(glm::vec3(
		cos(yawRad) * cos(pitchRad), 
		sin(pitchRad),              
		sin(yawRad) * cos(pitchRad)  
	));

	cam.right = glm::normalize(glm::cross(cam.forward, glm::vec3(0, 1, 0)));
	cam.up = glm::normalize(glm::cross(cam.right, cam.forward));
}
 inline void updateCameraMovement(GLFWwindow* window)
{
	 
	float speed = noRender.movementSpeed * noRender.deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.position += camera.forward * speed;
		//printf("pos %f %f %f \n", camera.position.x, camera.position.y, camera.position.z);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.position -= camera.forward * speed;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.position -= camera.right * speed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.position += camera.right * speed;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.position += camera.up * speed;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.position -= camera.up * speed;

	
}

 inline glm::mat4 getViewMatrix() {
	 return glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
 }
 inline glm::mat4 getProjMatrix() {
	 return glm::perspective(glm::radians(camera.fov), view.aspect, 0.1f, 10000.f);
 }
 inline void cameraLookAt(glm::vec3 target) {
	 glm::vec3 dir = glm::normalize(target - camera.position);
	 camera.pitch = glm::degrees(asin(dir.y));
	 camera.yaw = glm::degrees(atan2(dir.z, dir.x));
 }
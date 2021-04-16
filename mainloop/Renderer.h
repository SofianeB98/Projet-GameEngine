#pragma once


#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#if defined(_WIN32) && defined(_MSC_VER)
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#elif defined(__APPLE__)
#elif defined(__linux__)
#endif

#include <iostream>

#include "InfraStructure.h"
#include "../common/GLShader.h"
#include "smath.h"

namespace ESGI
{

	struct Renderer : public InfraStructure
	{
		GLShader defaultShader;
		uint32_t defaultShaderProgram;

		uint32_t cubeVAO;
		uint32_t cubeVBO;
		
		GLFWwindow* windowsContext;

		const uint32_t WIDTH = 800;
		const uint32_t HEIGHT = 600;

#pragma region CAMERA
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f);
		glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		float camSpeed = 5.f;
		float zoomMax = 80.0f, zoomMin = 1.0f;
		float lastXPos = 0.0f, lastYPos = 0.0f;
		float phi, theta;
		float sensitivityZoomCam = 0.50f, sensitivityMoveCam = 0.50f;
		float distanceBetweenCamAndTarget = 5.0f;
		bool firstMouse = true;

#pragma endregion 
		
		bool Initialize() final;
		void DeInitialize() final;

		void PreUpdate();
		void Update(float dt);
		void PostUpdate();

		bool WindowShouldClose() {
			return glfwWindowShouldClose(this->windowsContext);
		}

		void cursor_callback(GLFWwindow* window, double xpos, double ypos);
		void zoom_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	};
}

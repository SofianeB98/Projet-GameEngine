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

		bool Initialize() final;
		void DeInitialize() final;

		void PreUpdate();
		void Update();
		void PostUpdate();

		bool WindowShouldClose() {
			return glfwWindowShouldClose(this->windowsContext);
		}
	};
}

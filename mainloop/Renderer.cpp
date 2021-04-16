#include "Renderer.h"
#include "EcsBase.h"

#define _USE_MATH_DEFINES 1
#include <math.h>

#define BUFFER_OFFSET(i) ((void*)(i))

void ESGI::Renderer::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	if (state == GLFW_RELEASE)
	{
		lastXPos = xpos;
		lastYPos = ypos;
		return;
	}

	if (firstMouse) // initially set to true
	{
		lastXPos = xpos;
		lastYPos = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastXPos;
	float yoffset = lastYPos - ypos;
	lastXPos = xpos;
	lastYPos = ypos;

	xoffset *= sensitivityMoveCam;
	yoffset *= sensitivityMoveCam;

	theta += yoffset;
	phi += xoffset;

	if (phi > 180.0f)
		phi = -180.0f;
	if (phi < -180.0f)
		phi = 180.0f;

	if (theta > 89.0f)
		theta = 89.0;
	if (theta < -89.0f)
		theta = -89.0;

	float newTheta = theta * M_PI / 180.0f;
	float newPhi = phi * M_PI / 180.0f;

	glm::vec3 newpos(distanceBetweenCamAndTarget * std::cos(newTheta) * std::cos(newPhi),
		distanceBetweenCamAndTarget * std::sin(newTheta),
		distanceBetweenCamAndTarget * std::cos(newTheta) * std::sin(newPhi));

	camFront = glm::normalize(newpos);
	
	view = glm::lookAt(camTarget + camFront * distanceBetweenCamAndTarget, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
}

void ESGI::Renderer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Renderer* rd = (Renderer*)glfwGetWindowUserPointer(window);
	
	if (rd->distanceBetweenCamAndTarget >= rd->zoomMax - 0.1f && yoffset < 0)
		return;

	if (rd->distanceBetweenCamAndTarget <= rd->zoomMin + 0.1f && yoffset > 0)
		return;

	rd->distanceBetweenCamAndTarget -= yoffset * rd->sensitivityZoomCam;

	float newTheta = rd->theta * M_PI / 180.0f;
	float newPhi = rd->phi * M_PI / 180.0f;

	glm::vec3 newpos(rd->distanceBetweenCamAndTarget * std::cos(newTheta) * std::cos(newPhi),
		rd->distanceBetweenCamAndTarget * std::sin(newTheta),
		rd->distanceBetweenCamAndTarget * std::cos(newTheta) * std::sin(newPhi));

	rd->camFront = glm::normalize(newpos);

	rd->view = glm::lookAt(rd->camTarget + rd->camFront * rd->distanceBetweenCamAndTarget, rd->camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
}

bool ESGI::Renderer::Initialize()
{
	/* Initialize the library */
	if (!glfwInit())
		return false;

	/* Create a windowed mode window and its OpenGL context */

	this->windowsContext = glfwCreateWindow(WIDTH, HEIGHT, "Engine Window", NULL, NULL);
	if (!windowsContext)
	{
		glfwTerminate();
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(windowsContext);

	// Passe l'adresse de notre application a la fenetre
	glfwSetWindowUserPointer(windowsContext, &(*this));
	
	glfwSetScrollCallback(windowsContext, scroll_callback);
	glfwSetInputMode(windowsContext, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // permet d'utiliser les input de la souris

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "erreur d'initialisation de GLEW!" << std::endl;
	}

	// Creation des shader
	defaultShader.LoadFragmentShader("defaultShader.fs.glsl");
	defaultShader.LoadVertexShader("defaultShader.vs.glsl");
	defaultShader.Create();
	defaultShaderProgram = defaultShader.GetProgram();
	
	glUseProgram(defaultShaderProgram);
	
#ifdef WIN32
	wglSwapIntervalEXT(1);
#endif

	std::cout << "Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Vendor : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;

	
	glEnable(GL_DEPTH_TEST);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
										   
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
										   
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
										   
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
										   
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
										   
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};


	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	
	glBindVertexArray(cubeVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	int32_t positionLocation = glGetAttribLocation(defaultShaderProgram, "a_Position");
	int32_t texcoordsLocation = glGetAttribLocation(defaultShaderProgram, "a_TexCoords");
	int32_t normalLocation = glGetAttribLocation(defaultShaderProgram, "a_normals");
	
	// position attribute
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(positionLocation);
	// texture coord attribute
	glVertexAttribPointer(texcoordsLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(texcoordsLocation);
	// normal attribute
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(normalLocation);
	//glBindVertexArray(0);
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//view = glm::lookAt(glm::vec3(0.0f, 20.f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::lookAt(camTarget + camFront * distanceBetweenCamAndTarget, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(glm::radians(60.0f), (float)WIDTH / (float)HEIGHT, 0.03f, 1000.0f);
	
	return true;
}

void ESGI::Renderer::DeInitialize()
{
	glDeleteBuffers(1, &cubeVBO);
	glDeleteVertexArrays(1, &cubeVAO);
	
	defaultShader.Destroy();
	
	glfwTerminate();
}

void ESGI::Renderer::PreUpdate()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Defini le viewport en pleine fenetre
	glViewport(0, 0, WIDTH, HEIGHT);
}


void ESGI::Renderer::Update(float dt)
{
	// Do some stuff ?
	double xpos, ypos;
	//getting cursor position
	glfwGetCursorPos(windowsContext, &xpos, &ypos);
	
	cursor_callback(windowsContext, xpos, ypos);
	
	glUniformMatrix4fv(glGetUniformLocation(defaultShaderProgram, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(defaultShaderProgram, "projection"), 1, false, glm::value_ptr(projection));

	glUniform3f(glGetUniformLocation(defaultShaderProgram, "u_viewPos"), view[3][0], view[3][1], view[3][2]);
}


void ESGI::Renderer::PostUpdate()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(windowsContext);

	/* Poll for and process events */
	glfwPollEvents();
}
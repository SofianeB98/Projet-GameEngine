#include "Renderer.h"


#define BUFFER_OFFSET(i) ((void*)(i))

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

	glfwSetInputMode(windowsContext, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // permet d'utiliser les input de la souris
	/* Make the window's context current */
	glfwMakeContextCurrent(windowsContext);

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
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	
	glBindVertexArray(cubeVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	int32_t positionLocation = glGetAttribLocation(defaultShaderProgram, "a_Position");
	int32_t texcoordsLocation = glGetAttribLocation(defaultShaderProgram, "a_TexCoords");
	
	// position attribute
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(positionLocation);
	// texture coord attribute
	glVertexAttribPointer(texcoordsLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(texcoordsLocation);

	//glBindVertexArray(0);
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
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


void ESGI::Renderer::Update()
{
	// To some stuff ?
}


void ESGI::Renderer::PostUpdate()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(windowsContext);

	/* Poll for and process events */
	glfwPollEvents();
}
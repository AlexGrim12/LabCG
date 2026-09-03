#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <glew.h>
#include <glfw3.h>

const int WIDTH = 800, HEIGHT = 800;
GLuint VAO_letras, VBO_letras, shader;

static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,1.0f,1.0f,1.0f);	 			\n\
}";

void CrearLetras() {
	GLfloat vertices[] = {
		// Letra A (Low Poly tipo mosaico basado en la imagen)
		// Nivel 1 (Punta)
		-0.60f,  0.30f, 0.0f,  -0.68f, 0.15f, 0.0f,  -0.53f, 0.15f, 0.0f,
		// Nivel 2 (3 triangulos)
		-0.68f, 0.15f, 0.0f,  -0.75f,  0.00f, 0.0f,  -0.60f,  0.00f, 0.0f, // Izq-Up
		-0.68f, 0.15f, 0.0f,  -0.60f,  0.00f, 0.0f,  -0.53f, 0.15f, 0.0f, // Centro-Down
		-0.53f, 0.15f, 0.0f,  -0.60f,  0.00f, 0.0f,  -0.45f,  0.00f, 0.0f, // Der-Up
		// Nivel 3 (4 triangulos, el centro apunta hacia arriba y es un hueco)
		-0.75f,  0.00f, 0.0f,  -0.83f,-0.15f, 0.0f,  -0.68f,-0.15f, 0.0f, // Izq-Up
		-0.75f,  0.00f, 0.0f,  -0.68f,-0.15f, 0.0f,  -0.60f,  0.00f, 0.0f, // Izq-Mid-Down
		-0.60f,  0.00f, 0.0f,  -0.53f,-0.15f, 0.0f,  -0.45f,  0.00f, 0.0f, // Der-Mid-Down
		-0.45f,  0.00f, 0.0f,  -0.53f,-0.15f, 0.0f,  -0.38f,-0.15f, 0.0f, // Der-Up
		// Nivel 4 (Base: 5 triangulos, con espacios vacios separando las piernas del travesano)
		-0.83f,-0.15f, 0.0f,  -0.90f, -0.30f, 0.0f,  -0.75f, -0.30f, 0.0f, // Pierna Izq-Up
		-0.83f,-0.15f, 0.0f,  -0.75f, -0.30f, 0.0f,  -0.68f,-0.15f, 0.0f, // Pierna Izq-Down
		-0.68f,-0.15f, 0.0f,  -0.60f, -0.30f, 0.0f,  -0.53f,-0.15f, 0.0f, // Travesano central (Down)
		-0.53f,-0.15f, 0.0f,  -0.45f, -0.30f, 0.0f,  -0.38f,-0.15f, 0.0f, // Pierna Der-Down
		-0.38f,-0.15f, 0.0f,  -0.45f, -0.30f, 0.0f,  -0.30f, -0.30f, 0.0f, // Pierna Der-Up

		//Letra G (1)
		-0.13f, 0.21f, 0.0f,   0.13f, 0.21f, 0.0f,   0.13f, 0.28f, 0.0f,
		-0.13f, 0.21f, 0.0f,   0.13f, 0.28f, 0.0f,  -0.13f, 0.28f, 0.0f,
		-0.13f,-0.28f, 0.0f,  -0.06f,-0.28f, 0.0f,  -0.06f, 0.28f, 0.0f,
		-0.13f,-0.28f, 0.0f,  -0.06f, 0.28f, 0.0f,  -0.13f, 0.28f, 0.0f,
		-0.13f,-0.28f, 0.0f,   0.13f,-0.28f, 0.0f,   0.13f,-0.21f, 0.0f,
		-0.13f,-0.28f, 0.0f,   0.13f,-0.21f, 0.0f,  -0.13f,-0.21f, 0.0f,
		 0.00f,-0.04f, 0.0f,   0.13f,-0.04f, 0.0f,   0.13f, 0.04f, 0.0f,
		 0.00f,-0.04f, 0.0f,   0.13f, 0.04f, 0.0f,   0.00f, 0.04f, 0.0f,
		 0.06f,-0.21f, 0.0f,   0.13f,-0.21f, 0.0f,   0.13f,-0.04f, 0.0f,
		 0.06f,-0.21f, 0.0f,   0.13f,-0.04f, 0.0f,   0.06f,-0.04f, 0.0f,

		 //Letra G (2)
		  0.47f, 0.21f, 0.0f,   0.73f, 0.21f, 0.0f,   0.73f, 0.28f, 0.0f,
		  0.47f, 0.21f, 0.0f,   0.73f, 0.28f, 0.0f,   0.47f, 0.28f, 0.0f,
		  0.47f,-0.28f, 0.0f,   0.54f,-0.28f, 0.0f,   0.54f, 0.28f, 0.0f,
		  0.47f,-0.28f, 0.0f,   0.54f, 0.28f, 0.0f,   0.47f, 0.28f, 0.0f,
		  0.47f,-0.28f, 0.0f,   0.73f,-0.28f, 0.0f,   0.73f,-0.21f, 0.0f,
		  0.47f,-0.28f, 0.0f,   0.73f,-0.21f, 0.0f,   0.47f,-0.21f, 0.0f,
		  0.60f,-0.04f, 0.0f,   0.73f,-0.04f, 0.0f,   0.73f, 0.04f, 0.0f,
		  0.60f,-0.04f, 0.0f,   0.73f, 0.04f, 0.0f,   0.60f, 0.04f, 0.0f,
		  0.66f,-0.21f, 0.0f,   0.73f,-0.21f, 0.0f,   0.73f,-0.04f, 0.0f,
		  0.66f,-0.21f, 0.0f,   0.73f,-0.04f, 0.0f,   0.66f,-0.04f, 0.0f
	};
	glGenVertexArrays(1, &VAO_letras);
	glBindVertexArray(VAO_letras);

	glGenBuffers(1, &VBO_letras);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_letras);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = (GLint)strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}
}

int main()
{
	if (!glfwInit())
	{
		printf("Fallo inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fallo inicializacion de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, BufferWidth, BufferHeight);

	CrearLetras();
	CompileShaders();

	//Semilla aleatoria distinta en cada ejecucion
	srand((unsigned int)time(NULL));

	float bgR = 0.0f, bgG = 0.0f, bgB = 0.0f;
	double ultimoCambio = 0.0;

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		//Cada 2 segundos se elige un nuevo color de fondo al azar
		double tiempoActual = glfwGetTime();
		if (tiempoActual - ultimoCambio >= 2.0)
		{
			bgR = (rand() % 256) / 255.0f;
			bgG = (rand() % 256) / 255.0f;
			bgB = (rand() % 256) / 255.0f;
			ultimoCambio = tiempoActual;
		}
		glClearColor(bgR, bgG, bgB, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO_letras);
		glDrawArrays(GL_TRIANGLES, 0, 99);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}

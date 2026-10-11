#include "Window.h"

//las patas de las ruedas solo pueden girar 45 grados hacia adelante y hacia atras
static GLfloat limitar45(GLfloat a) { return a > 45.0f ? 45.0f : (a < -45.0f ? -45.0f : a); }

Window::Window()
{
	width = 800;
	height = 600;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = articulacion2 = articulacion3 = articulacion4 = articulacion5 = 0.0f;
	llanta1 = llanta2 = llanta3 = llanta4 = llanta5 = llanta6 = 0.0f;
	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = articulacion2 = articulacion3 = articulacion4 = articulacion5 = 0.0f;
	llanta1 = llanta2 = llanta3 = llanta4 = llanta5 = llanta6 = 0.0f;
	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 7: Iluminacion", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();
	//Atrapar el cursor dentro de la ventana para poder girar la camara sin limite
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	else if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	else if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}

	//teclas F,G,H,J,K mueven las articulaciones (con SHIFT giran al reves)
	if (action != GLFW_RELEASE)
	{
		GLfloat paso = (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		if (key == GLFW_KEY_F) theWindow->articulacion1 += paso;
		else if (key == GLFW_KEY_G) theWindow->articulacion2 += paso;
		else if (key == GLFW_KEY_H) theWindow->articulacion3 += paso;
		else if (key == GLFW_KEY_J) theWindow->articulacion4 += paso;
		else if (key == GLFW_KEY_K) theWindow->articulacion5 += paso;

		//teclas 1-6 giran cada pata de rueda, maximo 45 grados a cada lado (con SHIFT al reves)
		GLfloat vuelta = (mode & GLFW_MOD_SHIFT) ? -5.0f : 5.0f;
		if (key == GLFW_KEY_1) theWindow->llanta1 = limitar45(theWindow->llanta1 + vuelta);
		else if (key == GLFW_KEY_2) theWindow->llanta2 = limitar45(theWindow->llanta2 + vuelta);
		else if (key == GLFW_KEY_3) theWindow->llanta3 = limitar45(theWindow->llanta3 + vuelta);
		else if (key == GLFW_KEY_4) theWindow->llanta4 = limitar45(theWindow->llanta4 + vuelta);
		else if (key == GLFW_KEY_5) theWindow->llanta5 = limitar45(theWindow->llanta5 + vuelta);
		else if (key == GLFW_KEY_6) theWindow->llanta6 = limitar45(theWindow->llanta6 + vuelta);
	}


	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}

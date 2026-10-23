/*
Practica 8: Iluminacion 2 - nave espacial con luces y motores de plasma
- Nave importada flotando, con 1 luz puntual y 1 spotlight unidos a ella
- Cada motor tiene una textura de plasma azul y una luz puntual azul en su posicion
Teclas:
  L: prender/apagar al mismo tiempo la luz puntual y el spotlight de la nave
  1: motor 1 (luz azul + fuego)   2: motor 2 (luz azul + fuego)
  WASD y mouse: camara   Esc: salir
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "Skybox.h"
#include "Luces.h"

Window mainWindow;
std::vector<MeshModel*> meshListModel; // [0] piso, [1] llama (dos planos cruzados)
std::vector<Shader> shaderList;        // [0] con luces, [1] sin luces (para el fuego)
Camera camera;

Texture pisoTexture;
Texture fuegoTexture;

Model Nave_M;
Skybox skybox;

//Iluminacion
DirectionalLight mainLight;
PointLight luzNave;          //luz puntual unida a la nave
SpotLight spotNave;          //spotlight unido a la nave
PointLight luzMotor1;        //luz azul en la posicion del fuego del motor 1
PointLight luzMotor2;        //luz azul en la posicion del fuego del motor 2
Material materialBrillante;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

void CreateObjects()
{
	unsigned int floorIndices[] = { 0, 2, 1, 1, 2, 3 };
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	MeshModel* piso = new MeshModel();
	piso->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(piso);

	//llama: dos planos cruzados, de la tobera (z = 0) hacia atras (z = -1); v = 1 en la tobera
	unsigned int llamaIndices[] = { 0, 1, 2,  2, 3, 0,   4, 5, 6,  6, 7, 4 };
	GLfloat llamaVertices[] = {
		-0.5f, 0.0f,  0.0f,	0.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		 0.5f, 0.0f,  0.0f,	1.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		 0.5f, 0.0f, -1.0f,	1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		-0.5f, 0.0f, -1.0f,	0.0f, 0.0f,	0.0f, 1.0f, 0.0f,

		0.0f, -0.5f,  0.0f,	0.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		0.0f,  0.5f,  0.0f,	1.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		0.0f,  0.5f, -1.0f,	1.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, -1.0f,	0.0f, 0.0f,	1.0f, 0.0f, 0.0f
	};
	MeshModel* llama = new MeshModel();
	llama->CreateMeshModel(llamaVertices, llamaIndices, 64, 12);
	meshListModel.push_back(llama);
}

void CreateShaders()
{
	shaderList.reserve(2); //evita que el vector copie y destruya el primer shader al agregar el segundo
	Shader* conLuces = new Shader();
	conLuces->CreateFromFiles("shaders/shader_light.vert", "shaders/shader_light.frag");
	shaderList.push_back(*conLuces);
	Shader* sinLuces = new Shader();
	sinLuces->CreateFromFiles("shaders/shader_texture.vert", "shaders/shader_texture.frag");
	shaderList.push_back(*sinLuces);
}

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(2.0f, 5.0f, 8.5f), glm::vec3(0.0f, 1.0f, 0.0f), -101.0f, -8.0f, 0.3f, 0.5f);

	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	fuegoTexture = Texture("Textures/fuego_azul.png");
	fuegoTexture.LoadTextureA();

	Nave_M = Model();
	Nave_M.LoadModel("Models/nave_espacial.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");
	skybox = Skybox(skyboxFaces);

	//Luces (poca luz ambiente para que se vean las de la nave)
	//direccion = hacia donde viaja la luz
	mainLight = DirectionalLight(0.75f, 0.82f, 1.0f, 0.30f, 0.20f, 0.3f, -1.0f, -0.4f);
	//PointLight(color, ambiente, difusa, posicion, atenuacion)
	luzNave = PointLight(1.0f, 0.92f, 0.75f, 0.0f, 2.2f, 0.0f, 0.0f, 0.0f, 1.0f, 0.09f, 0.032f);
	//SpotLight(color, ambiente, difusa, posicion, direccion, atenuacion, angulo del cono)
	spotNave = SpotLight(1.0f, 1.0f, 0.90f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.75f, 0.66f, 1.0f, 0.02f, 0.004f, 30.0f);
	luzMotor1 = PointLight(0.25f, 0.50f, 1.0f, 0.0f, 3.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.09f, 0.05f);
	luzMotor2 = PointLight(0.25f, 0.50f, 1.0f, 0.0f, 3.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.09f, 0.05f);
	materialBrillante = Material(0.5f, 16.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	glm::mat4 model(1.0);
	glm::vec3 color(1.0f, 1.0f, 1.0f);

	//estados: luces de la nave, motor 1, motor 2 (los motores empiezan apagados)
	bool lucesNaveOn = true, motor1On = false, motor2On = false;
	int teclas[3] = { GLFW_KEY_L, GLFW_KEY_1, GLFW_KEY_2 };
	bool teclaPrev[3] = { false, false, false };

	//posicion de la salida de gases de cada motor (en el modelo de la nave) y direccion del escape (-z)
	glm::vec3 salidaMotor1(1.45f, -0.15f, -3.4f);
	glm::vec3 salidaMotor2(-1.45f, -0.15f, -3.4f);
	float largoLlama = 3.0f, anchoLlama = 1.1f;

	int cuadros = 0;
	printf("L: luces de la nave | 1: motor 1 | 2: motor 2\n");

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//se ignora el mouse en los primeros cuadros para que la camara no salte al abrir
		GLfloat dx = mainWindow.getXChange(), dy = mainWindow.getYChange();
		if (cuadros++ > 30) camera.mouseControl(dx, dy);

		//teclas que se leen una vez por pulsacion
		bool* keys = mainWindow.getsKeys();
		for (int i = 0; i < 3; i++)
		{
			bool presionada = keys[teclas[i]] && !teclaPrev[i];
			teclaPrev[i] = keys[teclas[i]];
			if (!presionada) continue;
			if (i == 0) lucesNaveOn = !lucesNaveOn;
			else if (i == 1) motor1On = !motor1On;
			else motor2On = !motor2On;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		//Matriz de la nave (padre de las luces y de las llamas): flota sobre el piso
		glm::mat4 naveMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.5f, -2.0f));
		naveMat = glm::rotate(naveMat, glm::radians(-125.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//Jerarquia: la posicion de cada luz sale de la matriz de la nave
		glm::vec3 posNave = glm::vec3(naveMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));          //luz puntual: bajo el vientre
		glm::vec3 posSpot = glm::vec3(naveMat * glm::vec4(0.0f, -0.45f, 3.6f, 1.0f));         //spotlight: en la nariz
		glm::vec3 dirSpot = glm::vec3(naveMat * glm::vec4(0.0f, -0.75f, 0.66f, 0.0f));        //hacia abajo y adelante
		glm::vec3 posFuego1 = glm::vec3(naveMat * glm::vec4(salidaMotor1 + glm::vec3(0.0f, 0.0f, -1.0f), 1.0f));
		glm::vec3 posFuego2 = glm::vec3(naveMat * glm::vec4(salidaMotor2 + glm::vec3(0.0f, 0.0f, -1.0f), 1.0f));
		luzNave.SetPosition(posNave.x, posNave.y, posNave.z);
		spotNave.SetFlash(posSpot, dirSpot);
		luzMotor1.SetPosition(posFuego1.x, posFuego1.y, posFuego1.z);
		luzMotor2.SetPosition(posFuego2.x, posFuego2.y, posFuego2.z);

		//Dibujo con luces
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//Solo las luces encendidas se mandan al shader; el conteo baja cuando se apaga una
		GLuint prog = shaderList[0].getShaderID();
		mainLight.UseLight(prog);
		PointLight* puntualesActivas[3];
		int nPuntuales = 0;
		if (lucesNaveOn) puntualesActivas[nPuntuales++] = &luzNave;
		if (motor1On) puntualesActivas[nPuntuales++] = &luzMotor1;
		if (motor2On) puntualesActivas[nPuntuales++] = &luzMotor2;
		for (int i = 0; i < nPuntuales; i++)
			puntualesActivas[i]->UseLight(prog, i);
		int nSpots = 0;
		if (lucesNaveOn) { spotNave.UseLight(prog, 0); nSpots = 1; }
		glUniform1i(glGetUniformLocation(prog, "pointLightCount"), nPuntuales);
		glUniform1i(glGetUniformLocation(prog, "spotLightCount"), nSpots);
		materialBrillante.UseMaterial(prog);

		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		meshListModel[0]->RenderMeshModel();

		//Nave
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(naveMat));
		Nave_M.RenderModel();

		//Fuego de los motores (sin luces, con mezcla aditiva)
		if (motor1On || motor2On)
		{
			shaderList[1].UseShader();
			uniformModel = shaderList[1].GetModelLocation();
			glUniformMatrix4fv(shaderList[1].GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(shaderList[1].GetViewLocation(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glm::vec3 brillo(1.6f, 1.6f, 1.8f);
			glUniform3fv(shaderList[1].getColorLocation(), 1, glm::value_ptr(brillo));
			fuegoTexture.UseTexture();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glDepthMask(GL_FALSE);
			for (int m = 0; m < 2; m++)
			{
				bool encendido = (m == 0) ? motor1On : motor2On;
				if (!encendido) continue;
				glm::vec3 salida = (m == 0) ? salidaMotor1 : salidaMotor2;
				//la llama titila un poco
				float titileo = 1.0f + 0.07f * sin(now * 30.0f + m * 2.0f) + 0.04f * sin(now * 47.0f + m);
				model = glm::translate(naveMat, salida);
				model = glm::scale(model, glm::vec3(anchoLlama, anchoLlama, largoLlama * titileo));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				meshListModel[1]->RenderMeshModel();
			}
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}

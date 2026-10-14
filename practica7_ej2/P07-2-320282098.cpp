/*
Practica 7: Iluminacion - Ejercicio 2: luces de faro (spotlight)
Cada faro es hijo de su objeto: usa la misma matriz del padre, por eso lo sigue.
Flechas arriba/abajo: rover | flechas der/izq: avion adelante/atras | AvPag/RePag: avion sube/baja
7, 8, 9: prender/apagar faros | 1 a 6: patas del rover | WASD y mouse: camara
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

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // [0] piso, [1] cubo del foco
std::vector<Shader> shaderList;
Camera camera;

Texture plainTexture;
Texture pisoTexture;

//Modelos
Model Rover_Cuerpo, Rover_Brazo;
Model Pata_DelDer, Pata_DelIzq, Pata_MedDer, Pata_MedIzq, Pata_TrasDer, Pata_TrasIzq;
Model Avion_M;

Skybox skybox;

//Iluminacion
DirectionalLight mainLight;
SpotLight faroRover;        //hijo del rover
SpotLight faroAvionAbajo;   //hijo del avion
SpotLight faroAvionFrente;  //hijo del avion
Material materialBrillante;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

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

	//cubo del foco de cada faro
	unsigned int cuboIndices[] = {
		0, 1, 2,  2, 3, 0,
		4, 5, 6,  6, 7, 4,
		8, 9, 10,  10, 11, 8,
		12, 13, 14,  14, 15, 12,
		16, 17, 18,  18, 19, 16,
		20, 21, 22,  22, 23, 20
	};
	GLfloat cuboVertices[] = {
		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,	0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,	0.0f, 0.0f, -1.0f,

		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,	-1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f,	-1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,	0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,	0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,	0.0f, 1.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,	0.0f, -1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	0.0f, -1.0f, 0.0f
	};
	MeshModel* cubo = new MeshModel();
	cubo->CreateMeshModel(cuboVertices, cuboIndices, 192, 36);
	meshListModel.push_back(cubo);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//limita el recorrido
static float limitar(float v, float mn, float mx) { return v < mn ? mn : (v > mx ? mx : v); }

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 6.0f, 18.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -12.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	//Rover: cuerpo, brazo y 6 patas
	Rover_Cuerpo = Model();   Rover_Cuerpo.LoadModel("Models/RoverCuerpo.obj");
	Rover_Brazo = Model();    Rover_Brazo.LoadModel("Models/RoverBrazo.obj");
	Pata_DelDer = Model();    Pata_DelDer.LoadModel("Models/RoverPataDelDer.obj");
	Pata_DelIzq = Model();    Pata_DelIzq.LoadModel("Models/RoverPataDelIzq.obj");
	Pata_MedDer = Model();    Pata_MedDer.LoadModel("Models/RoverPataMedDer.obj");
	Pata_MedIzq = Model();    Pata_MedIzq.LoadModel("Models/RoverPataMedIzq.obj");
	Pata_TrasDer = Model();   Pata_TrasDer.LoadModel("Models/RoverPataTrasDer.obj");
	Pata_TrasIzq = Model();   Pata_TrasIzq.LoadModel("Models/RoverPataTrasIzq.obj");
	//Avion
	Avion_M = Model();
	Avion_M.LoadModel("Models/avion_bulldog.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");
	skybox = Skybox(skyboxFaces);

	//Luces (poca luz ambiente para que se vean los faros)
	//direccion = hacia donde viaja la luz
	mainLight = DirectionalLight(0.75f, 0.82f, 1.0f, 0.28f, 0.22f, 0.3f, -1.0f, -0.4f);
	//SpotLight(color, ambiente, difusa, posicion, direccion, atenuacion, angulo del cono)
	faroRover = SpotLight(1.0f, 0.95f, 0.80f, 0.0f, 4.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.03f, 0.008f, 28.0f);
	faroAvionAbajo = SpotLight(1.0f, 1.0f, 0.85f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.02f, 0.004f, 30.0f);
	faroAvionFrente = SpotLight(0.70f, 0.85f, 1.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.02f, 0.004f, 32.0f);
	materialBrillante = Material(0.5f, 16.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	//posicion de rover y avion
	float roverX = -5.0f;
	float avionX = -10.0f, avionY = 5.0f;
	bool faroOn[3] = { true, true, true };
	bool teclaPrev[3] = { false, false, false };

	printf("Flechas arriba/abajo: rover | Flechas der/izq: avion adelante/atras | AvPag/RePag: avion sube/baja | 7,8,9: faros\n");

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Movimiento con el teclado
		bool* keys = mainWindow.getsKeys();
		float paso = 0.08f * deltaTime;
		if (keys[GLFW_KEY_UP])        roverX += paso;
		if (keys[GLFW_KEY_DOWN])      roverX -= paso;
		if (keys[GLFW_KEY_RIGHT])     avionX += paso;
		if (keys[GLFW_KEY_LEFT])      avionX -= paso;
		if (keys[GLFW_KEY_PAGE_UP])   avionY += paso;
		if (keys[GLFW_KEY_PAGE_DOWN]) avionY -= paso;
		roverX = limitar(roverX, -16.0f, 16.0f);
		avionX = limitar(avionX, -16.0f, 16.0f);
		avionY = limitar(avionY, 2.5f, 14.0f);
		int teclas[3] = { GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9 };
		for (int i = 0; i < 3; i++)
		{
			if (keys[teclas[i]] && !teclaPrev[i]) faroOn[i] = !faroOn[i];
			teclaPrev[i] = keys[teclas[i]];
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//Matrices de los padres
		glm::mat4 roverMat = glm::translate(glm::mat4(1.0f), glm::vec3(roverX, -2.0f, 0.0f));
		roverMat = glm::scale(roverMat, glm::vec3(0.5f, 0.5f, 0.5f));

		glm::mat4 avionMat = glm::translate(glm::mat4(1.0f), glm::vec3(avionX, avionY, -5.0f));
		avionMat = glm::rotate(avionMat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //nariz hacia +x
		avionMat = glm::scale(avionMat, glm::vec3(0.8f, 0.8f, 0.8f));

		//Jerarquia: matriz del faro = matriz del padre * matriz local
		//rover: al frente
		glm::mat4 faroRoverMat = glm::translate(roverMat, glm::vec3(6.0f, 4.2f, 0.0f));
		//avion abajo: en el vientre
		glm::mat4 faroAbajoMat = glm::translate(avionMat, glm::vec3(0.0f, -0.95f, 0.2f));
		//avion adelante: en la nariz
		glm::mat4 faroFrenteMat = glm::translate(avionMat, glm::vec3(0.0f, 0.0f, 2.35f));

		glm::vec3 posRover = glm::vec3(faroRoverMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec3 dirRover = glm::normalize(glm::vec3(faroRoverMat * glm::vec4(1.0f, -0.30f, 0.0f, 0.0f)));
		glm::vec3 posAbajo = glm::vec3(faroAbajoMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec3 dirAbajo = glm::normalize(glm::vec3(faroAbajoMat * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));
		glm::vec3 posFrente = glm::vec3(faroFrenteMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glm::vec3 dirFrente = glm::normalize(glm::vec3(faroFrenteMat * glm::vec4(0.0f, -0.35f, 1.0f, 0.0f)));

		faroRover.SetFlash(posRover, dirRover);
		faroAvionAbajo.SetFlash(posAbajo, dirAbajo);
		faroAvionFrente.SetFlash(posFrente, dirFrente);
		faroRover.SetOn(faroOn[0]);
		faroAvionAbajo.SetOn(faroOn[1]);
		faroAvionFrente.SetOn(faroOn[2]);

		//Luces al shader
		GLuint prog = shaderList[0].getShaderID();
		mainLight.UseLight(prog);
		faroRover.UseLight(prog, 0);
		faroAvionAbajo.UseLight(prog, 1);
		faroAvionFrente.UseLight(prog, 2);
		glUniform1i(glGetUniformLocation(prog, "pointLightCount"), 0);
		glUniform1i(glGetUniformLocation(prog, "spotLightCount"), 3);
		materialBrillante.UseMaterial(prog);

		//Piso
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		meshListModel[0]->RenderMeshModel();

		//Rover: el cuerpo es el padre
		model = roverMat;
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_Cuerpo.RenderModelColor(uniformColor);

		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_Brazo.RenderModelColor(uniformColor);

		float pivoteDelantero = 5.55f;
		float pivoteTrasero = -3.04f;

		model = modelaux;
		model = glm::translate(model, glm::vec3(pivoteDelantero, 3.3f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta1()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-pivoteDelantero, -3.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_DelDer.RenderModelColor(uniformColor);

		model = modelaux;
		model = glm::translate(model, glm::vec3(pivoteDelantero, 3.3f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta2()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-pivoteDelantero, -3.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_DelIzq.RenderModelColor(uniformColor);

		model = modelaux;
		model = glm::translate(model, glm::vec3(pivoteTrasero, 3.3f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta3()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-pivoteTrasero, -3.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_MedDer.RenderModelColor(uniformColor);

		model = modelaux;
		model = glm::translate(model, glm::vec3(pivoteTrasero, 3.3f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta4()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-pivoteTrasero, -3.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_MedIzq.RenderModelColor(uniformColor);

		model = modelaux;
		model = glm::translate(model, glm::vec3(pivoteTrasero, 3.3f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta5()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-pivoteTrasero, -3.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_TrasDer.RenderModelColor(uniformColor);

		model = modelaux;
		model = glm::translate(model, glm::vec3(pivoteTrasero, 3.3f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getllanta6()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-pivoteTrasero, -3.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_TrasIzq.RenderModelColor(uniformColor);

		//Avion
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(avionMat));
		Avion_M.RenderModel();

		//Focos con la matriz de cada faro
		plainTexture.UseTexture();
		glm::vec3 focoColor = glm::vec3(8.0f, 8.0f, 6.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(focoColor));
		model = glm::scale(faroRoverMat, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListModel[1]->RenderMeshModel();
		model = glm::scale(faroAbajoMat, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListModel[1]->RenderMeshModel();
		model = glm::scale(faroFrenteMat, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshListModel[1]->RenderMeshModel();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}

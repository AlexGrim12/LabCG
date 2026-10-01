/*
Practica 6: Rover, Holocron y Satelite al mismo tiempo

ROVER (igual que la practica 5): teclas 1-6 giran cada pata de rueda, max 45 grados (SHIFT al reves)

HOLOCRON: teclas 7,8,9,0,U,I,O,P giran cada una de las 8 esquinas alrededor del centro del Holocron (SHIFT al reves)

SATELITE:
  flechas izquierda/derecha: mover en X        flechas arriba/abajo: mover en Z        AvPag/RePag: mover en Y
  teclas Z,X,C,V: giran cada panel solar en su propia bisagra con el cuerpo (SHIFT al reves)
*/
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
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshListColor;
std::vector<MeshModel*> meshListModel;
std::vector<Shader> shaderList;

Camera camera;

//--- Rover (partes separadas, igual que practica 5) ---
Model Rover_Cuerpo, Rover_Brazo;
Model Pata_DelDer, Pata_DelIzq, Pata_MedDer, Pata_MedIzq, Pata_TrasDer, Pata_TrasIzq;

//--- Holocron (cuerpo + 8 esquinas) ---
Model Holo_Cuerpo;
Model Holo_Esquina[8];

//--- Satelite (cuerpo + 4 paneles solares) ---
Model Sat_Cuerpo;
Model Sat_PanelPosX, Sat_PanelNegX, Sat_PanelPosY, Sat_PanelNegY;

Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

static const char* vShader = "shaders/shader_m.vert";
static const char* fShader = "shaders/shader_m.frag";


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
	meshListModel.push_back(piso); //indice 0
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 10.0f, 28.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -16.0f, 0.3f, 0.3f);

	//--- Cargar Rover por partes ---
	Rover_Cuerpo.LoadModel("Models/RoverCuerpo.obj");
	Rover_Brazo.LoadModel("Models/RoverBrazo.obj");
	Pata_DelDer.LoadModel("Models/RoverPataDelDer.obj");
	Pata_DelIzq.LoadModel("Models/RoverPataDelIzq.obj");
	Pata_MedDer.LoadModel("Models/RoverPataMedDer.obj");
	Pata_MedIzq.LoadModel("Models/RoverPataMedIzq.obj");
	Pata_TrasDer.LoadModel("Models/RoverPataTrasDer.obj");
	Pata_TrasIzq.LoadModel("Models/RoverPataTrasIzq.obj");

	//--- Cargar Holocron: cuerpo + 8 esquinas (A..H) ---
	Holo_Cuerpo.LoadModel("Models/HolocronCuerpo.obj");
	const char* nombresEsquina[8] = { "A","B","C","D","E","F","G","H" };
	for (int i = 0; i < 8; i++)
	{
		std::string ruta = std::string("Models/HolocronEsq") + nombresEsquina[i] + ".obj";
		Holo_Esquina[i].LoadModel(ruta);
	}
	//direccion del centro del Holocron hacia cada esquina (A..H), en el mismo orden que se exportaron
	glm::vec3 dirEsquina[8] = {
		glm::vec3(-1,-1,-1), glm::vec3(-1,-1, 1), glm::vec3(-1, 1,-1), glm::vec3(-1, 1, 1),
		glm::vec3(1,-1,-1), glm::vec3(1,-1, 1), glm::vec3(1, 1,-1), glm::vec3(1, 1, 1)
	};

	//--- Cargar Satelite: cuerpo + 4 paneles ---
	Sat_Cuerpo.LoadModel("Models/SatCuerpo.obj");
	Sat_PanelPosX.LoadModel("Models/SatPanelPosX.obj");
	Sat_PanelNegX.LoadModel("Models/SatPanelNegX.obj");
	Sat_PanelPosY.LoadModel("Models/SatPanelPosY.obj");
	Sat_PanelNegY.LoadModel("Models/SatPanelNegY.obj");

	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//piso
		glm::vec3 colorPiso(0.5f, 0.5f, 0.5f);
		model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPiso));
		meshListModel[0]->RenderMeshModel();

		//====================== ROVER ======================
		glm::mat4 roverBase = glm::translate(glm::mat4(1.0), glm::vec3(-10.0f, -2.0f, -1.5f));
		model = roverBase;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_Cuerpo.RenderModelColor(uniformColor);

		model = roverBase;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_Brazo.RenderModelColor(uniformColor);

		float pivoteDelantero = 5.55f, pivoteTrasero = -3.04f;
		Model* patas[6] = { &Pata_DelDer, &Pata_DelIzq, &Pata_MedDer, &Pata_MedIzq, &Pata_TrasDer, &Pata_TrasIzq };
		float pivoteX[6] = { pivoteDelantero, pivoteDelantero, pivoteTrasero, pivoteTrasero, pivoteTrasero, pivoteTrasero };
		float anguloLlanta[6] = { mainWindow.getllanta1(), mainWindow.getllanta2(), mainWindow.getllanta3(),
								   mainWindow.getllanta4(), mainWindow.getllanta5(), mainWindow.getllanta6() };
		for (int i = 0; i < 6; i++)
		{
			model = roverBase;
			model = glm::translate(model, glm::vec3(pivoteX[i], 3.3f, 0.0f));
			model = glm::rotate(model, glm::radians(anguloLlanta[i]), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-pivoteX[i], -3.3f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			patas[i]->RenderModelColor(uniformColor);
		}

		//====================== HOLOCRON ======================
		//cada esquina gira alrededor del CENTRO del holocron (no alrededor de si misma):
		//se traslada al centro, se rota sobre un eje tangencial a la esquina, y se regresa
		glm::mat4 holoBase = glm::mat4(1.0);
		holoBase = glm::translate(holoBase, glm::vec3(0.0f, 3.0f, -3.0f));
		holoBase = glm::scale(holoBase, glm::vec3(0.14f, 0.14f, 0.14f));

		//el modelo del Holocron no trae color (solo texturas que este shader simple no usa),
		//asi que se le pone un color fijo distinto a cada pieza con RenderModel (sin leer el material)
		model = holoBase;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glm::vec3 colorCuerpoHolo(0.3f, 0.32f, 0.36f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoHolo));
		Holo_Cuerpo.RenderModel();

		glm::vec3 colorEsquina[8] = {
			glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.4f,1.0f), glm::vec3(1.0f,1.0f,0.0f),
			glm::vec3(1.0f,0.0f,1.0f), glm::vec3(0.0f,1.0f,1.0f), glm::vec3(1.0f,0.55f,0.0f), glm::vec3(0.6f,0.0f,1.0f)
		};
		for (int i = 0; i < 8; i++)
		{
			glm::vec3 dir = glm::normalize(dirEsquina[i]);
			glm::vec3 eje = glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)));
			model = holoBase; //el centro del Holocron esta en el origen de este sistema, por eso se rota aqui directo
			model = glm::rotate(model, glm::radians(mainWindow.getesquina(i)), eje);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(colorEsquina[i]));
			Holo_Esquina[i].RenderModel();
		}

		//====================== SATELITE ======================
		glm::mat4 satBase = glm::mat4(1.0);
		satBase = glm::translate(satBase, glm::vec3(10.0f, 4.0f, -3.0f));
		satBase = glm::translate(satBase, mainWindow.getSatPos()); //se mueve con las flechas y AvPag/RePag
		satBase = glm::scale(satBase, glm::vec3(1.3f, 1.3f, 1.3f));

		//el satelite tampoco trae color (solo texturas); se le ponen colores fijos igual que al Holocron
		glm::vec3 colorCuerpoSat(0.75f, 0.6f, 0.2f); //dorado
		model = satBase;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorCuerpoSat));
		Sat_Cuerpo.RenderModel();

		//cada panel solo gira en su propia bisagra (el borde donde se une al cuerpo), no alrededor de todo el satelite
		glm::vec3 bisagraPosX(0.3705f, 0.0f, 0.297f), bisagraNegX(-0.3705f, 0.0f, 0.297f);
		glm::vec3 bisagraPosY(0.0f, 0.3705f, 0.297f), bisagraNegY(0.0f, -0.3705f, 0.297f);
		glm::vec3 colorPanelPosX(0.0f, 0.4f, 1.0f), colorPanelNegX(1.0f, 1.0f, 0.0f);
		glm::vec3 colorPanelPosY(0.0f, 1.0f, 0.0f), colorPanelNegY(1.0f, 0.0f, 0.0f);

		model = satBase;
		model = glm::translate(model, bisagraPosX);
		model = glm::rotate(model, glm::radians(mainWindow.getpanel(1)), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -bisagraPosX);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPanelPosX));
		Sat_PanelPosX.RenderModel();

		model = satBase;
		model = glm::translate(model, bisagraNegX);
		model = glm::rotate(model, glm::radians(mainWindow.getpanel(0)), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -bisagraNegX);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPanelNegX));
		Sat_PanelNegX.RenderModel();

		model = satBase;
		model = glm::translate(model, bisagraPosY);
		model = glm::rotate(model, glm::radians(mainWindow.getpanel(3)), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -bisagraPosY);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPanelPosY));
		Sat_PanelPosY.RenderModel();

		model = satBase;
		model = glm::translate(model, bisagraNegY);
		model = glm::rotate(model, glm::radians(mainWindow.getpanel(2)), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -bisagraNegY);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPanelNegY));
		Sat_PanelNegY.RenderModel();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}

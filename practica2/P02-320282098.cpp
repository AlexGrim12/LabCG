//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos: un vertex shader por cada color, todos usan el mismo fragment shader
static const char* vShaderVerde = "shaders/shaderVerde.vert";
static const char* vShaderAzul = "shaders/shaderAzul.vert";
static const char* vShaderCafe = "shaders/shaderCafe.vert";
static const char* vShaderMagenta = "shaders/shaderMagenta.vert";

//color café/marrón en RGB : 0.478, 0.255, 0.067

using std::vector;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *piramidetriangular = new Mesh();
	piramidetriangular->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(piramidetriangular);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		0, 1, 2,
		2, 3, 0,

		1, 5, 6,
		6, 2, 1,
		
		7, 6, 5,
		5, 4, 7,
		
		4, 0, 3,
		3, 7, 4,
		
		4, 5, 1,
		1, 0, 4,
		
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

//Letras A, G y R hechas con triángulos
void CrearLetras()
{
	GLfloat vertices_letras[] = {
		//X			Y			Z			R		G		B
		//Letra A (verde)
		-0.60f,	 0.30f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.68f,	 0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.52f,	 0.15f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.68f,	 0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.75f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.60f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.68f,	 0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.60f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.52f,	 0.15f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.52f,	 0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.60f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.45f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.75f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.83f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.68f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.75f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.68f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.60f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.60f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.52f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.45f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.45f,	 0.00f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.52f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.37f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.83f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.90f,	-0.30f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.75f,	-0.30f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.83f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.75f,	-0.30f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.68f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.68f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.60f,	-0.30f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.52f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.52f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.45f,	-0.30f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.37f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,

		-0.37f,	-0.15f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.45f,	-0.30f,		0.0f,			0.0f,	1.0f,	0.0f,
		-0.30f,	-0.30f,		0.0f,			0.0f,	1.0f,	0.0f,

		//Letra G (azul)
		-0.13f,	 0.21f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	 0.21f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	 0.28f,		0.0f,			0.0f,	0.0f,	1.0f,

		-0.13f,	 0.21f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	 0.28f,		0.0f,			0.0f,	0.0f,	1.0f,
		-0.13f,	 0.28f,		0.0f,			0.0f,	0.0f,	1.0f,

		-0.13f,	-0.28f,		0.0f,			0.0f,	0.0f,	1.0f,
		-0.06f,	-0.28f,		0.0f,			0.0f,	0.0f,	1.0f,
		-0.06f,	 0.28f,		0.0f,			0.0f,	0.0f,	1.0f,

		-0.13f,	-0.28f,		0.0f,			0.0f,	0.0f,	1.0f,
		-0.06f,	 0.28f,		0.0f,			0.0f,	0.0f,	1.0f,
		-0.13f,	 0.28f,		0.0f,			0.0f,	0.0f,	1.0f,

		-0.13f,	-0.28f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	-0.28f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	-0.21f,		0.0f,			0.0f,	0.0f,	1.0f,

		-0.13f,	-0.28f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	-0.21f,		0.0f,			0.0f,	0.0f,	1.0f,
		-0.13f,	-0.21f,		0.0f,			0.0f,	0.0f,	1.0f,

		 0.00f,	-0.04f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	-0.04f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	 0.04f,		0.0f,			0.0f,	0.0f,	1.0f,

		 0.00f,	-0.04f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	 0.04f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.00f,	 0.04f,		0.0f,			0.0f,	0.0f,	1.0f,

		 0.06f,	-0.21f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	-0.21f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	-0.04f,		0.0f,			0.0f,	0.0f,	1.0f,

		 0.06f,	-0.21f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.13f,	-0.04f,		0.0f,			0.0f,	0.0f,	1.0f,
		 0.06f,	-0.04f,		0.0f,			0.0f,	0.0f,	1.0f,

		//Letra R (magenta)
		 0.47f,	-0.28f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.54f,	-0.28f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.54f,	 0.28f,		0.0f,			1.0f,	0.0f,	1.0f,

		 0.47f,	-0.28f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.54f,	 0.28f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.47f,	 0.28f,		0.0f,			1.0f,	0.0f,	1.0f,

		 0.47f,	 0.21f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	 0.21f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	 0.28f,		0.0f,			1.0f,	0.0f,	1.0f,

		 0.47f,	 0.21f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	 0.28f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.47f,	 0.28f,		0.0f,			1.0f,	0.0f,	1.0f,

		 0.66f,	 0.04f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	 0.04f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	 0.21f,		0.0f,			1.0f,	0.0f,	1.0f,

		 0.66f,	 0.04f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	 0.21f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.66f,	 0.21f,		0.0f,			1.0f,	0.0f,	1.0f,

		 0.47f,	-0.03f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	-0.03f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	 0.04f,		0.0f,			1.0f,	0.0f,	1.0f,

		 0.47f,	-0.03f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	 0.04f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.47f,	 0.04f,		0.0f,			1.0f,	0.0f,	1.0f,

		 0.60f,	-0.03f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.67f,	-0.03f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	-0.28f,		0.0f,			1.0f,	0.0f,	1.0f,

		 0.60f,	-0.03f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.73f,	-0.28f,		0.0f,			1.0f,	0.0f,	1.0f,
		 0.66f,	-0.28f,		0.0f,			1.0f,	0.0f,	1.0f,
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 594);
	meshColorList.push_back(letras);
}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//shaders de un solo color para las pirámides y cubos
	Shader* shaderVerde = new Shader(); //índice 2
	shaderVerde->CreateFromFiles(vShaderVerde, fShader);
	shaderList.push_back(*shaderVerde);

	Shader* shaderAzul = new Shader(); //índice 3
	shaderAzul->CreateFromFiles(vShaderAzul, fShader);
	shaderList.push_back(*shaderAzul);

	Shader* shaderCafe = new Shader(); //índice 4
	shaderCafe->CreateFromFiles(vShaderCafe, fShader);
	shaderList.push_back(*shaderCafe);

	Shader* shaderMagenta = new Shader(); //índice 5
	shaderMagenta->CreateFromFiles(vShaderMagenta, fShader);
	shaderList.push_back(*shaderMagenta);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearPiramideCuadrangular(); //índice 2 en MeshList
	CrearLetras(); //usa MeshColor, índice 0 en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana (color de fondo distinto a blanco, negro y a los colores de las figuras)
		glClearColor(0.95f, 0.95f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		//////////////// LETRAS A G R: shader de color con índice 1 en ShaderList ////////////////
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.4f, -7.0f));
		model = glm::scale(model, glm::vec3(3.6f, 3.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[0]->RenderMeshColor();

		//////////////// FIGURAS VERDES: shader con índice 2 ////////////////
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//Figura 1: pirámide de abajo (invertida)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.6f, -1.98f, -7.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//Figura 2: pirámide esquina superior izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.58f, -1.02f, -7.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.55f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//Figura 2: pirámide esquina inferior derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.58f, -2.18f, -7.0f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.55f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//Figura 3: pirámide inferior izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.15f, -1.93f, -7.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//////////////// FIGURAS AZULES: shader con índice 3 ////////////////
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//Figura 1: pirámide de arriba (invertida)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.6f, -0.38f, -7.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//Figura 2: cubo girado 45 grados (rombo)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.6f, -7.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Figura 3: pirámide del centro (invertida)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.6f, -1.93f, -7.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//////////////// FIGURAS CAFÉS: shader con índice 4 ////////////////
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, -7.0f));
		model = glm::scale(model, glm::vec3(8.4f, 0.24f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Figura 1: barra izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.15f, -1.18f, -7.0f));
		model = glm::scale(model, glm::vec3(0.16f, 2.4f, 0.16f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Figura 1: barra derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.05f, -1.18f, -7.0f));
		model = glm::scale(model, glm::vec3(0.16f, 2.4f, 0.16f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//Figura 2: cubo chico del centro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.6f, -6.4f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		//////////////// FIGURAS MAGENTA: shader con índice 5 ////////////////
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//Figura 1: pirámide de en medio (invertida)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.6f, -1.18f, -7.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//Figura 2: pirámide esquina superior derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.58f, -1.02f, -7.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.55f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//Figura 2: pirámide esquina inferior izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.58f, -2.18f, -7.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.55f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//Figura 3: pirámide de arriba
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.6f, -1.03f, -7.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		//Figura 3: pirámide inferior derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.05f, -1.93f, -7.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

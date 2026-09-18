//pr�ctica 3: Modelado Geom�trico y C�mara Sint�tica.
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al c�digo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<MeshColor*> meshColorList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pir�mide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidet = new Mesh();
	piramidet->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(piramidet);

}


//funci�n para crear pir�mide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,//frontal
		3,2,4,//izquierda
		2,1,4,//trasera
		1,0,4,//derecha
		0,1,2,//abajo1
		0,2,3//abajo2

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramidec = new Mesh();
	piramidec->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramidec);
}

//pir�mide de base cuadrada con un color distinto por cada cara (ejercicio de la estrella de 8 pir�mides)
//las 4 caras triangulares son: roja, verde, amarilla, magenta. La cara cuadrada (base) es azul.
//no usa �ndices: cada tri�ngulo lleva sus propios 3 v�rtices para que no comparta color con las caras vecinas
void CrearPiramideEstrella()
{
	GLfloat vertices_estrella[] = {
		//X		Y		Z			R		G		B
		//cara frontal: roja
		0.5f,	-0.5f,	0.5f,		1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,	0.5f,		1.0f,	0.0f,	0.0f,
		0.0f,	0.5f,	0.0f,		1.0f,	0.0f,	0.0f,
		//cara izquierda: verde
		-0.5f,	-0.5f,	0.5f,		0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	1.0f,	0.0f,
		0.0f,	0.5f,	0.0f,		0.0f,	1.0f,	0.0f,
		//cara trasera: amarilla
		-0.5f,	-0.5f,	-0.5f,		1.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,	-0.5f,		1.0f,	1.0f,	0.0f,
		0.0f,	0.5f,	0.0f,		1.0f,	1.0f,	0.0f,
		//cara derecha: magenta
		0.5f,	-0.5f,	-0.5f,		1.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,		1.0f,	0.0f,	1.0f,
		0.0f,	0.5f,	0.0f,		1.0f,	0.0f,	1.0f,
		//base (2 tri�ngulos): azul
		0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,		0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,		0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,	0.5f,		0.0f,	0.0f,	1.0f,
	};
	MeshColor* piramideestrella = new MeshColor();
	piramideestrella->CreateMeshColor(vertices_estrella, 108); //18 v�rtices x 6 floats (x,y,z,r,g,b)
	meshColorList.push_back(piramideestrella);
}

//calcula una matriz de rotaci�n que orienta el eje Y local hacia la direcci�n "dir"
//se usa para apuntar cada pir�mide de la estrella hacia afuera, sobre su propia diagonal
glm::mat4 OrientarHaciaDireccion(glm::vec3 dir)
{
	dir = glm::normalize(dir);
	glm::vec3 arriba = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 eje = glm::cross(arriba, dir);
	float angulo = acos(glm::clamp(glm::dot(arriba, dir), -1.0f, 1.0f));
	return glm::rotate(glm::mat4(1.0f), angulo, glm::normalize(eje));
}



/*
Crear cilindro, cono y esferas con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los v�rtices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el c�rculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los v�rtices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//cilindro con tri�ngulos correctamente armados (paredes + las 2 tapas), para dibujarse con RenderMesh (GL_TRIANGLES)
//se usa para el cuerpo del cohete
void CrearCilindroSolido(int res, float R)
{
	vector<GLfloat> vertices;
	vector<unsigned int> indices;
	GLfloat dt = 2 * PI / res;

	//borde de abajo (0..res-1) y borde de arriba (res..2*res-1)
	for (int n = 0; n < res; n++)
	{
		vertices.push_back(R * cos(n * dt));
		vertices.push_back(-0.5f);
		vertices.push_back(R * sin(n * dt));
	}
	for (int n = 0; n < res; n++)
	{
		vertices.push_back(R * cos(n * dt));
		vertices.push_back(0.5f);
		vertices.push_back(R * sin(n * dt));
	}
	//centro de cada tapa
	int centroAbajo = 2 * res;
	int centroArriba = 2 * res + 1;
	vertices.push_back(0.0f); vertices.push_back(-0.5f); vertices.push_back(0.0f);
	vertices.push_back(0.0f); vertices.push_back(0.5f); vertices.push_back(0.0f);

	for (int n = 0; n < res; n++)
	{
		int ni = (n + 1) % res;
		//pared lateral: 2 tri�ngulos por cada segmento
		indices.push_back(n); indices.push_back(ni); indices.push_back(res + n);
		indices.push_back(ni); indices.push_back(res + ni); indices.push_back(res + n);
		//tapa de abajo
		indices.push_back(centroAbajo); indices.push_back(ni); indices.push_back(n);
		//tapa de arriba
		indices.push_back(centroArriba); indices.push_back(res + n); indices.push_back(res + ni);
	}

	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//funci�n para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//c�lculo del paso interno en la circunferencia y variables que almacenar�n cada coordenada de cada v�rtice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los v�rtices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearCilindro(5, 1.0f);//�ndice 2 en MeshList
	CrearCono(25, 2.0f);//�ndice 3 en MeshList
	CrearPiramideCuadrangular();//�ndice 4 en MeshList
	CrearCilindroSolido(24, 1.0f);//�ndice 5 en MeshList, cilindro liso para el cuerpo del cohete
	CrearCono(24, 1.0f);//�ndice 6 en MeshList, cono liso para la nariz del cohete
	CrearPiramideEstrella();//�ndice 0 en MeshColorList, pir�mide con sus 5 caras de colores distintos
	CreateShaders();
	
	

	/*C�mara se usa el comando: glm::lookAt(vector de posici�n, vector de orientaci�n, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posici�n,
	glm::vec3 vector up,
	GlFloat yaw rotaci�n para girar hacia la derecha e izquierda
	GlFloat pitch rotaci�n para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posici�n inicial est� en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//C�mara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//la l�nea de proyecci�n solo se manda una vez a menos que en tiempo de ejecuci�n
		//se programe cambio entre proyecci�n ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geom�tricas cilindro, cono
		//sp.render(); //dibuja esfera
		
		
		//ejercicio: recrear la figura central (el rombo) de la pr�ctica pasada en 3D,
		//cada pir�mide de la esquina es de un color diferente (verde, azul, cafe, magenta)
		//y se agrega el piso de color negro
		//todo el conjunto se mueve junto, como una sola pieza, con las mismas teclas E, R, T
		glm::mat4 modelBase = glm::mat4(1.0);
		modelBase = glm::translate(modelBase, glm::vec3(0.0f, 0.0f, -6.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelBase = glm::rotate(modelBase, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		//cubo girado 45 grados que forma el rombo
		model = modelBase;
		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); //azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//cubo chico del centro, pegado a la cara frontal del rombo para que se vea, no al centro exacto (si no, quedaria escondido adentro)
		model = modelBase;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.55f));
		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.5f, 0.0f); //olivo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//las 4 esquinas usan la piramide TRIANGULAR (no la de base cuadrada) para que
		//se vea como un triangulo recto que calza justo con el borde del cubo, sin huecos
		//escala (1.4, 0.7, 0.3) hace que el triangulo del frente quede con angulo recto en la punta

		//esquina superior izquierda del cuadrado: amarillo
		model = modelBase;
		model = glm::translate(model, glm::vec3(-0.62f, 0.62f, 0.0f));
		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.4f, 0.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f); //amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		//esquina superior derecha del cuadrado: rojo
		model = modelBase;
		model = glm::translate(model, glm::vec3(0.62f, 0.62f, 0.0f));
		model = glm::rotate(model, -45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.4f, 0.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		//esquina inferior izquierda del cuadrado: morado
		model = modelBase;
		model = glm::translate(model, glm::vec3(-0.62f, -0.62f, 0.0f));
		model = glm::rotate(model, 135.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.4f, 0.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.5f, 0.0f, 0.5f); //morado
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		//esquina inferior derecha del cuadrado: verde
		model = modelBase;
		model = glm::translate(model, glm::vec3(0.62f, -0.62f, 0.0f));
		model = glm::rotate(model, -135.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.4f, 0.7f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f); //verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		//piso negro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -6.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.3f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 0.0f); //negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ejercicio: cohete espacial hecho con instancias de piramides, cilindros, esferas, conos y cubos
		//todo el cohete se mueve junto, como una sola pieza, con las mismas teclas E, R, T
		glm::mat4 coheteBase = glm::mat4(1.0);
		coheteBase = glm::translate(coheteBase, glm::vec3(-6.0f, 0.0f, -6.0f));
		coheteBase = glm::rotate(coheteBase, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		coheteBase = glm::rotate(coheteBase, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		coheteBase = glm::rotate(coheteBase, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		//cuerpo: cilindro
		model = coheteBase;
		model = glm::scale(model, glm::vec3(0.5f, 2.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.8f, 0.8f, 0.8f); //gris claro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[5]->RenderMesh();

		//nariz: cono, se coloca justo arriba del cuerpo (cuerpo llega hasta y=1.25)
		model = coheteBase;
		model = glm::translate(model, glm::vec3(0.0f, 1.65f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.8f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[6]->RenderMeshGeometry();

		//ventanas: esferas chicas pegadas al cuerpo
		color = glm::vec3(0.6f, 0.9f, 1.0f); //celeste
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = coheteBase;
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = coheteBase;
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		//aletas: 4 pir�mides triangulares aplanadas, una por cada lado de la base del cuerpo
		color = glm::vec3(0.2f, 0.2f, 0.8f); //azul oscuro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		for (int angulo = 0; angulo < 360; angulo += 90)
		{
			model = coheteBase;
			model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.55f, -1.0f, 0.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.6f, 0.3f, 0.9f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[1]->RenderMesh();
		}

		//motores: 3 cubos chicos en la base del cuerpo
		color = glm::vec3(0.3f, 0.3f, 0.3f); //gris oscuro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		for (int angulo = 0; angulo < 360; angulo += 120)
		{
			model = coheteBase;
			model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.25f, -1.4f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.2f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[0]->RenderMesh();
		}

		//ejercicio: uni�n de 8 pir�mides formando una estrella de 8 puntas
		//cada pir�mide tiene sus 4 caras triangulares de colores distintos (rojo, verde, amarillo, magenta)
		//y su cara cuadrada (base) azul, oculta hacia el centro de la estrella
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		uniformView = shaderList[1].getViewLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		glm::mat4 estrellaBase = glm::mat4(1.0);
		estrellaBase = glm::translate(estrellaBase, glm::vec3(6.0f, 0.0f, -6.0f));
		estrellaBase = glm::rotate(estrellaBase, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		estrellaBase = glm::rotate(estrellaBase, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		estrellaBase = glm::rotate(estrellaBase, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		//las 8 direcciones diagonales (+-1,+-1,+-1) son las 8 puntas de la estrella
		for (int dx = -1; dx <= 1; dx += 2)
		for (int dy = -1; dy <= 1; dy += 2)
		for (int dz = -1; dz <= 1; dz += 2)
		{
			glm::vec3 direccion = glm::vec3((float)dx, (float)dy, (float)dz);
			model = estrellaBase * OrientarHaciaDireccion(direccion);
			model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshColorList[0]->RenderMeshColor();
		}

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		
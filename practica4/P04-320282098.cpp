/*Pr�ctica 4: Modelado Jer�rquico.
Se implementa el uso de matrices adicionales para almacenar informaci�n de transformaciones geom�tricas que se quiere
heredar entre diversas instancias para que est�n unidas
Teclas de la F a la K para rotaciones de articulaciones del brazo
Teclas del 1 al 6 para girar cada una de las 6 llantas por separado
(con SHIFT las teclas F-K y 1-6 giran al reves)
*/
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
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
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
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos din�micos vector creados en el Semestre 2023 - 1 : por S�nchez P�rez Omar Alejandro
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


//cilindro con paredes y tapas bien armadas (se dibuja con RenderMesh)
void CrearCilindroSolido(int res, float R)
{
	vector<GLfloat> vertices;
	vector<unsigned int> indices;
	GLfloat dt = 2 * PI / res;
	for (int n = 0; n < res; n++) { vertices.push_back(R * cos(n * dt)); vertices.push_back(-0.5f); vertices.push_back(R * sin(n * dt)); }
	for (int n = 0; n < res; n++) { vertices.push_back(R * cos(n * dt)); vertices.push_back(0.5f); vertices.push_back(R * sin(n * dt)); }
	vertices.push_back(0.0f); vertices.push_back(-0.5f); vertices.push_back(0.0f);
	vertices.push_back(0.0f); vertices.push_back(0.5f); vertices.push_back(0.0f);
	for (int n = 0; n < res; n++)
	{
		int ni = (n + 1) % res;
		indices.push_back(n); indices.push_back(ni); indices.push_back(res + n);
		indices.push_back(ni); indices.push_back(res + ni); indices.push_back(res + n);
		indices.push_back(2 * res); indices.push_back(ni); indices.push_back(n);
		indices.push_back(2 * res + 1); indices.push_back(res + n); indices.push_back(res + ni);
	}
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, (unsigned int)vertices.size(), (unsigned int)indices.size());
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

//funci�n para crear pir�mide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resoluci�n (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//�ndice 0 en MeshList
	CrearPiramideTriangular();//�ndice 1 en MeshList
	CrearCilindroSolido(18, 1.0f);//�ndice 2 en MeshList
	CrearCono(25, 2.0f);//�ndice 3 en MeshList
	CrearPiramideCuadrangular();//�ndice 4 en MeshList
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

	camera = Camera(glm::vec3(8.0f, 10.0f, 26.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -11.0f, 0.3f, 0.3f);

	
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
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux2(1.0);

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
		glClearColor(0.6f, 0.62f, 0.66f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();


		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//funciones para dibujar cada pieza con su escala y color
		auto cubo = [&](glm::mat4 m, glm::vec3 esc, glm::vec3 c) {
			m = glm::scale(m, esc);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m));
			glUniform3fv(uniformColor, 1, glm::value_ptr(c));
			meshList[0]->RenderMesh(); };
		auto cilindro = [&](glm::mat4 m, glm::vec3 esc, glm::vec3 c) {
			m = glm::scale(m, esc);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m));
			glUniform3fv(uniformColor, 1, glm::value_ptr(c));
			meshList[2]->RenderMesh(); };
		auto esfera = [&](glm::mat4 m, glm::vec3 esc, glm::vec3 c) {
			m = glm::scale(m, esc);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m));
			glUniform3fv(uniformColor, 1, glm::value_ptr(c));
			sp.render(); };
		auto cono = [&](glm::mat4 m, glm::vec3 esc, glm::vec3 c) {
			m = glm::scale(m, esc);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m));
			glUniform3fv(uniformColor, 1, glm::value_ptr(c));
			meshList[3]->RenderMeshGeometry(); };

		glm::vec3 blanco(0.95f, 0.95f, 0.95f), gris(0.5f, 0.5f, 0.53f), grisclaro(0.75f, 0.75f, 0.78f);
		glm::vec3 negro(0.08f, 0.08f, 0.08f), naranja(1.0f, 0.65f, 0.0f), colcabina(0.68f, 0.7f, 0.75f);

		//piso
		cubo(glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -0.1f, -4.0f)), glm::vec3(60.0f, 0.2f, 40.0f), glm::vec3(0.42f, 0.38f, 0.34f));

		//CABINA
		glm::mat4 cab = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 4.0f, -4.0f));
		cab = glm::translate(cab, glm::vec3(1.0f, 2.0f, 0.0f));
		cubo(cab, glm::vec3(8.0f, 4.0f, 6.0f), colcabina);
		cubo(glm::translate(cab, glm::vec3(-1.0f, 0.5f, 3.02f)), glm::vec3(3.0f, 0.4f, 0.05f), naranja);
		cubo(glm::translate(cab, glm::vec3(0.5f, 2.03f, 0.8f)), glm::vec3(3.0f, 0.05f, 1.2f), negro);
		cilindro(glm::translate(cab, glm::vec3(-2.5f, 3.5f, -1.5f)), glm::vec3(0.3f, 3.0f, 0.3f), gris);
		cubo(glm::translate(cab, glm::vec3(-2.5f, 5.2f, -1.5f)), glm::vec3(0.9f, 0.5f, 0.7f), blanco);
		esfera(glm::translate(cab, glm::vec3(-2.0f, 5.2f, -1.75f)), glm::vec3(0.18f, 0.18f, 0.18f), negro);
		esfera(glm::translate(cab, glm::vec3(-2.0f, 5.2f, -1.25f)), glm::vec3(0.18f, 0.18f, 0.18f), negro);
		cilindro(glm::translate(cab, glm::vec3(1.8f, 2.15f, -1.5f)), glm::vec3(1.8f, 0.2f, 1.8f), grisclaro);

		//BASE
		glm::mat4 base = glm::translate(cab, glm::vec3(-1.0f, -2.75f, 0.0f));
		cubo(base, glm::vec3(10.0f, 1.5f, 8.0f), blanco);

		//6 LLANTAS: delanteras, de en medio y de atras, cada una gira por separado con las teclas 1-6
		//1=del-izq 2=del-der 3=medio-izq 4=medio-der 5=atras-izq 6=atras-der (con SHIFT giran al reves)
		float vueltaLlanta[6] = {
			mainWindow.getllanta1(), mainWindow.getllanta2(),
			mainWindow.getllanta3(), mainWindow.getllanta4(),
			mainWindow.getllanta5(), mainWindow.getllanta6() };
		float ejeX[3] = { 3.8f, 0.0f, -3.8f }; //posicion en X de cada eje: delantero, medio, atras
		for (int eje = 0; eje < 3; eje++)
		{
			for (int lado = -1; lado <= 1; lado += 2)
			{
				int numLlanta = eje * 2 + (lado < 0 ? 0 : 1); //indice 0..5 en vueltaLlanta
				cubo(glm::translate(base, glm::vec3(ejeX[eje], -1.25f, lado * 4.5f)), glm::vec3(0.8f, 2.0f, 1.0f), gris);
				glm::mat4 rueda = glm::translate(base, glm::vec3(ejeX[eje], -1.75f, lado * 5.0f));
				//el giro se aplica antes de acostar la llanta, para que gire sobre su propio eje (el eje)
				rueda = glm::rotate(rueda, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				rueda = glm::rotate(rueda, glm::radians(vueltaLlanta[numLlanta]), glm::vec3(0.0f, 1.0f, 0.0f));
				cilindro(rueda, glm::vec3(1.5f, 1.2f, 1.5f), negro);
				cilindro(glm::translate(rueda, glm::vec3(0.0f, lado * 0.65f, 0.0f)), glm::vec3(0.7f, 0.25f, 0.7f), grisclaro);
			}
		}

		//BRAZO CON PINZA (F,G,H,J,K; con SHIFT al reves)
		glm::mat4 hombro = glm::translate(base, glm::vec3(-3.5f, 1.25f, 3.5f));
		cubo(hombro, glm::vec3(1.2f, 1.0f, 1.2f), gris);
		hombro = glm::translate(hombro, glm::vec3(0.0f, 0.8f, 0.0f));
		hombro = glm::rotate(hombro, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 1.0f, 0.0f));
		hombro = glm::rotate(hombro, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		esfera(hombro, glm::vec3(0.6f, 0.6f, 0.6f), gris);

		glm::mat4 brazo1 = glm::rotate(hombro, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		brazo1 = glm::translate(brazo1, glm::vec3(0.0f, 2.3f, 0.0f));
		cilindro(brazo1, glm::vec3(0.5f, 4.0f, 0.5f), blanco);

		glm::mat4 codo = glm::translate(brazo1, glm::vec3(0.0f, 2.3f, 0.0f));
		codo = glm::rotate(codo, glm::radians(-45.0f + mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		esfera(codo, glm::vec3(0.6f, 0.6f, 0.6f), gris);

		glm::mat4 brazo2 = glm::translate(codo, glm::vec3(0.0f, 2.3f, 0.0f));
		cilindro(brazo2, glm::vec3(0.5f, 4.0f, 0.5f), blanco);

		glm::mat4 muneca = glm::translate(brazo2, glm::vec3(0.0f, 2.3f, 0.0f));
		muneca = glm::rotate(muneca, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		esfera(muneca, glm::vec3(0.5f, 0.5f, 0.5f), gris);

		cubo(glm::translate(muneca, glm::vec3(0.0f, 0.8f, 0.0f)), glm::vec3(1.0f, 1.2f, 1.0f), gris);
		float abre = 20.0f + mainWindow.getarticulacion4();
		for (int lado = -1; lado <= 1; lado += 2)
		{
			glm::mat4 dedo = glm::translate(muneca, glm::vec3(lado * 0.35f, 1.4f, 0.0f));
			dedo = glm::rotate(dedo, glm::radians(-lado * abre), glm::vec3(0.0f, 0.0f, 1.0f));
			dedo = glm::translate(dedo, glm::vec3(0.0f, 0.7f, 0.0f));
			cubo(dedo, glm::vec3(0.25f, 1.4f, 0.5f), naranja);
		}

		//SONDA ESPACIAL, modelada jerarquicamente: cuerpo -> mastil -> antena, y cuerpo -> soporte/panel (x2)
		glm::vec3 azulpanel(0.12f, 0.22f, 0.5f);
		glm::mat4 sonda = glm::translate(glm::mat4(1.0), glm::vec3(16.0f, 6.0f, -4.0f));
		cubo(sonda, glm::vec3(2.0f, 2.0f, 2.0f), grisclaro); //cuerpo

		glm::mat4 mastil = glm::translate(sonda, glm::vec3(0.0f, 1.3f, 0.0f));
		cilindro(mastil, glm::vec3(0.15f, 1.2f, 0.15f), gris);
		glm::mat4 antena = glm::translate(mastil, glm::vec3(0.0f, 0.9f, 0.0f));
		antena = glm::rotate(antena, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //abre hacia arriba
		cono(antena, glm::vec3(1.6f, 0.7f, 1.6f), blanco); //antena parabolica

		for (int lado = -1; lado <= 1; lado += 2)
		{
			glm::mat4 soporte = glm::translate(sonda, glm::vec3(lado * 0.8f, 0.0f, 0.0f));
			soporte = glm::rotate(soporte, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			cilindro(soporte, glm::vec3(0.12f, 1.0f, 0.12f), gris);
			cubo(glm::translate(sonda, glm::vec3(lado * 2.4f, 0.0f, 0.0f)), glm::vec3(3.0f, 0.1f, 1.6f), azulpanel);
		}
		esfera(glm::translate(sonda, glm::vec3(0.0f, 0.0f, 1.2f)), glm::vec3(0.3f, 0.3f, 0.3f), negro); //sensor frontal

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

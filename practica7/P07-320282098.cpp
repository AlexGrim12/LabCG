/*
Práctica 6: Texturizado
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
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include "Luces.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz

std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture holocronTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Holocron_M;


Skybox skybox;

//Iluminacion
DirectionalLight mainLight;
PointLight pointLights[3];
Material materialBrillante;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};


	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// Atlas "star_wars_dado.png" (cruz de 4x3 celdas), un logo por cara
		// celda (col,fila): arriba (1,0) | izq (0,1) frente (1,1) der (2,1) atras (3,1) | abajo (1,2)
		// front: Rebel Alliance
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.25f,  0.3333f,	0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.50f,	0.3333f,	0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.50f,	0.6667f,	0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.25f,	0.6667f,	0.0f,	0.0f,	-1.0f,	//3
		// right: Galactic Empire
		0.5f, -0.5f,  0.5f,	    0.50f,  0.3333f,	-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.75f,	0.3333f,	-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.75f,	0.6667f,	-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.50f,	0.6667f,	-1.0f,	0.0f,	0.0f,
		// back: Galactic Republic
		-0.5f, -0.5f, -0.5f,	1.00f,  0.3333f,	0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.75f,	0.3333f,	0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.75f,	0.6667f,	0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	1.00f,	0.6667f,	0.0f,	0.0f,	1.0f,

		// left: Sith Empire
		-0.5f, -0.5f,  -0.5f,	0.00f,  0.3333f,	1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.25f,	0.3333f,	1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.25f,	0.6667f,	1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.00f,	0.6667f,	1.0f,	0.0f,	0.0f,

		// bottom: Mandalorian Crest
		-0.5f, -0.5f,  0.5f,	0.25f,  0.3333f,	0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.50f,	0.3333f,	0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.50f,	0.0000f,	0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.25f,	0.0000f,	0.0f,	1.0f,	0.0f,

		//UP: First Order
		 -0.5f, 0.5f,  0.5f,	0.25f,  0.6667f,	0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.50f,	0.6667f,	0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.50f,	1.0000f,	0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.25f,	1.0000f,	0.0f,	-1.0f,	0.0f,

	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	// Holocron texturizado por codigo: cada triangulo tiene sus propios vertices (x y z, S T, nx ny nz)
	// Atlas "holocron_tex.png" (4x4 celdas): fila 0 = caras +x,-x,+y,-y | fila 1 = caras +z,-z y 2 esquinas | filas 2-3 = esquinas
	// Las 6 caras grandes usan proyeccion plana (S,T = coordenadas de la cara); cada esquina usa su propia celda.
	unsigned int holocron_indices[84];
	for (unsigned int i = 0; i < 84; i++)
		holocron_indices[i] = i;

	GLfloat holocron_vertices[] = {
	-2.453980f, -0.063388f, -2.477451f,	0.62695f, 0.70975f,	0.5290f, 0.5889f, 0.6110f,
	-1.185055f, -2.530499f, -1.198261f,	0.62371f, 0.58249f,	0.5290f, 0.5889f, 0.6110f,
	-2.517274f, -2.579095f, 0.002115f,	0.69843f, 0.58107f,	0.5290f, 0.5889f, 0.6110f,
	-2.453980f, -0.063388f, -2.477451f,	0.26440f, 0.87217f,	0.9985f, -0.0402f, 0.0381f,
	-2.646786f, -0.139047f, 2.493757f,	0.48633f, 0.86879f,	0.9985f, -0.0402f, 0.0381f,
	-2.451372f, 2.421759f, 0.077538f,	0.37846f, 0.98311f,	0.9985f, -0.0402f, 0.0381f,
	2.521206f, 2.448241f, 0.063388f,	0.30053f, 0.41868f,	-0.5661f, -0.5994f, 0.5660f,
	2.534944f, -0.061146f, -2.580280f,	0.37803f, 0.28806f,	-0.5661f, -0.5994f, 0.5660f,
	0.049649f, 2.440128f, -2.417217f,	0.44644f, 0.41826f,	-0.5661f, -0.5994f, 0.5660f,
	0.049649f, 2.440128f, -2.417217f,	0.05200f, 0.41828f,	0.5737f, -0.5874f, 0.5708f,
	-2.453980f, -0.063388f, -2.477451f,	0.12380f, 0.28938f,	0.5737f, -0.5874f, 0.5708f,
	-2.451372f, 2.421759f, 0.077538f,	0.19919f, 0.41734f,	0.5737f, -0.5874f, 0.5708f,
	-0.140696f, -2.601934f, 2.558561f,	0.11872f, 0.50884f,	0.0224f, -0.0036f, -0.9997f,
	0.051152f, 2.399781f, 2.545043f,	0.12728f, 0.73213f,	0.0224f, -0.0036f, -0.9997f,
	-2.646786f, -0.139047f, 2.493757f,	0.00684f, 0.61879f,	0.0224f, -0.0036f, -0.9997f,
	-2.451372f, 2.421759f, 0.077538f,	0.05468f, 0.16938f,	0.5649f, -0.5887f, -0.5782f,
	-2.646786f, -0.139047f, 2.493757f,	0.11921f, 0.03738f,	0.5649f, -0.5887f, -0.5782f,
	0.051152f, 2.399781f, 2.545043f,	0.20111f, 0.16824f,	0.5649f, -0.5887f, -0.5782f,
	0.051152f, 2.399781f, 2.545043f,	0.30084f, 0.16644f,	-0.5732f, -0.5771f, -0.5818f,
	2.593003f, -0.085222f, 2.505755f,	0.37743f, 0.03965f,	-0.5732f, -0.5771f, -0.5818f,
	2.521206f, 2.448241f, 0.063388f,	0.44673f, 0.16891f,	-0.5732f, -0.5771f, -0.5818f,
	-2.646786f, -0.139047f, 2.493757f,	0.62482f, 0.45911f,	0.5934f, 0.5893f, -0.5483f,
	-1.328995f, -2.578386f, 1.297838f,	0.62536f, 0.33330f,	0.5934f, 0.5893f, -0.5483f,
	-0.140696f, -2.601934f, 2.558561f,	0.69754f, 0.33201f,	0.5934f, 0.5893f, -0.5483f,
	2.593003f, -0.085222f, 2.505755f,	0.87782f, 0.45938f,	-0.5933f, 0.5680f, -0.5705f,
	1.385028f, -2.549476f, 1.308677f,	0.87533f, 0.33465f,	-0.5933f, 0.5680f, -0.5705f,
	2.632201f, -2.553892f, 0.007226f,	0.95034f, 0.33229f,	-0.5933f, 0.5680f, -0.5705f,
	2.534944f, -0.061146f, -2.580280f,	0.87620f, 0.70971f,	-0.5377f, 0.5767f, 0.6150f,
	1.344182f, -2.542571f, -1.294387f,	0.87580f, 0.58313f,	-0.5377f, 0.5767f, 0.6150f,
	0.081432f, -2.513204f, -2.426005f,	0.94644f, 0.58325f,	-0.5377f, 0.5767f, 0.6150f,
	0.081432f, -2.513204f, -2.426005f,	0.37136f, 0.51280f,	0.0641f, -0.0014f, 0.9979f,
	0.049649f, 2.440128f, -2.417217f,	0.37278f, 0.73393f,	0.0641f, -0.0014f, 0.9979f,
	2.534944f, -0.061146f, -2.580280f,	0.26183f, 0.62227f,	0.0641f, -0.0014f, 0.9979f,
	1.344182f, -2.542571f, -1.294387f,	0.93501f, 0.81721f,	-0.0107f, 0.9999f, 0.0028f,
	1.385028f, -2.549476f, 1.308677f,	0.93683f, 0.93342f,	-0.0107f, 0.9999f, 0.0028f,
	-1.328995f, -2.578386f, 1.297838f,	0.81567f, 0.93294f,	-0.0107f, 0.9999f, 0.0028f,
	2.632201f, -2.553892f, 0.007226f,	0.12468f, 0.76099f,	-0.9997f, -0.0223f, 0.0063f,
	2.521206f, 2.448241f, 0.063388f,	0.12217f, 0.98430f,	-0.9997f, -0.0223f, 0.0063f,
	2.593003f, -0.085222f, 2.505755f,	0.01314f, 0.87120f,	-0.9997f, -0.0223f, 0.0063f,
	0.049649f, 2.440128f, -2.417217f,	0.62722f, 0.98291f,	0.0114f, -0.9999f, -0.0081f,
	0.051152f, 2.399781f, 2.545043f,	0.62728f, 0.76138f,	0.0114f, -0.9999f, -0.0081f,
	2.521206f, 2.448241f, 0.063388f,	0.73755f, 0.87217f,	0.0114f, -0.9999f, -0.0081f,
	0.049649f, 2.440128f, -2.417217f,	0.62722f, 0.98291f,	-0.0008f, -1.0000f, -0.0081f,
	-2.451372f, 2.421759f, 0.077538f,	0.51556f, 0.87154f,	-0.0008f, -1.0000f, -0.0081f,
	0.051152f, 2.399781f, 2.545043f,	0.62728f, 0.76138f,	-0.0008f, -1.0000f, -0.0081f,
	0.081432f, -2.513204f, -2.426005f,	0.37136f, 0.51280f,	-0.0221f, -0.0019f, 0.9998f,
	-2.453980f, -0.063388f, -2.477451f,	0.48455f, 0.62217f,	-0.0221f, -0.0019f, 0.9998f,
	0.049649f, 2.440128f, -2.417217f,	0.37278f, 0.73393f,	-0.0221f, -0.0019f, 0.9998f,
	2.632201f, -2.553892f, 0.007226f,	0.12468f, 0.76099f,	-0.9996f, -0.0224f, 0.0160f,
	2.534944f, -0.061146f, -2.580280f,	0.24019f, 0.87227f,	-0.9996f, -0.0224f, 0.0160f,
	2.521206f, 2.448241f, 0.063388f,	0.12217f, 0.98430f,	-0.9996f, -0.0224f, 0.0160f,
	-0.140696f, -2.601934f, 2.558561f,	0.11872f, 0.50884f,	-0.0174f, -0.0020f, -0.9998f,
	2.593003f, -0.085222f, 2.505755f,	0.24076f, 0.62120f,	-0.0174f, -0.0020f, -0.9998f,
	0.051152f, 2.399781f, 2.545043f,	0.12728f, 0.73213f,	-0.0174f, -0.0020f, -0.9998f,
	-2.453980f, -0.063388f, -2.477451f,	0.26440f, 0.87217f,	0.9992f, 0.0133f, 0.0390f,
	-2.517274f, -2.579095f, 0.002115f,	0.37509f, 0.75986f,	0.9992f, 0.0133f, 0.0390f,
	-2.646786f, -0.139047f, 2.493757f,	0.48633f, 0.86879f,	0.9992f, 0.0133f, 0.0390f,
	-2.453980f, -0.063388f, -2.477451f,	0.62695f, 0.70975f,	0.5580f, 0.5897f, 0.5839f,
	0.081432f, -2.513204f, -2.426005f,	0.55025f, 0.58445f,	0.5580f, 0.5897f, 0.5839f,
	-1.185055f, -2.530499f, -1.198261f,	0.62371f, 0.58249f,	0.5580f, 0.5897f, 0.5839f,
	-2.646786f, -0.139047f, 2.493757f,	0.62482f, 0.45911f,	0.5952f, 0.5894f, -0.5462f,
	-2.517274f, -2.579095f, 0.002115f,	0.55210f, 0.33319f,	0.5952f, 0.5894f, -0.5462f,
	-1.328995f, -2.578386f, 1.297838f,	0.62536f, 0.33330f,	0.5952f, 0.5894f, -0.5462f,
	2.593003f, -0.085222f, 2.505755f,	0.87782f, 0.45938f,	-0.5337f, 0.5666f, -0.6278f,
	-0.140696f, -2.601934f, 2.558561f,	0.79335f, 0.32966f,	-0.5337f, 0.5666f, -0.6278f,
	1.385028f, -2.549476f, 1.308677f,	0.87533f, 0.33465f,	-0.5337f, 0.5666f, -0.6278f,
	2.534944f, -0.061146f, -2.580280f,	0.87620f, 0.70971f,	-0.5782f, 0.5766f, 0.5772f,
	2.632201f, -2.553892f, 0.007226f,	0.79956f, 0.58106f,	-0.5782f, 0.5766f, 0.5772f,
	1.344182f, -2.542571f, -1.294387f,	0.87580f, 0.58313f,	-0.5782f, 0.5766f, 0.5772f,
	-1.328995f, -2.578386f, 1.297838f,	0.81567f, 0.93294f,	-0.0202f, 0.9996f, 0.0180f,
	-2.517274f, -2.579095f, 0.002115f,	0.76262f, 0.87509f,	-0.0202f, 0.9996f, 0.0180f,
	-1.185055f, -2.530499f, -1.198261f,	0.82210f, 0.82151f,	-0.0202f, 0.9996f, 0.0180f,
	-1.185055f, -2.530499f, -1.198261f,	0.82210f, 0.82151f,	0.0055f, 0.9998f, 0.0198f,
	0.081432f, -2.513204f, -2.426005f,	0.87864f, 0.76670f,	0.0055f, 0.9998f, 0.0198f,
	1.344182f, -2.542571f, -1.294387f,	0.93501f, 0.81721f,	0.0055f, 0.9998f, 0.0198f,
	1.344182f, -2.542571f, -1.294387f,	0.93501f, 0.81721f,	0.0062f, 1.0000f, 0.0026f,
	2.632201f, -2.553892f, 0.007226f,	0.99251f, 0.87532f,	0.0062f, 1.0000f, 0.0026f,
	1.385028f, -2.549476f, 1.308677f,	0.93683f, 0.93342f,	0.0062f, 1.0000f, 0.0026f,
	1.385028f, -2.549476f, 1.308677f,	0.93683f, 0.93342f,	-0.0108f, 0.9995f, 0.0288f,
	-0.140696f, -2.601934f, 2.558561f,	0.86872f, 0.98922f,	-0.0108f, 0.9995f, 0.0288f,
	-1.328995f, -2.578386f, 1.297838f,	0.81567f, 0.93294f,	-0.0108f, 0.9995f, 0.0288f,
	-1.328995f, -2.578386f, 1.297838f,	0.81567f, 0.93294f,	0.0055f, 0.9998f, 0.0195f,
	-1.185055f, -2.530499f, -1.198261f,	0.82210f, 0.82151f,	0.0055f, 0.9998f, 0.0195f,
	1.344182f, -2.542571f, -1.294387f,	0.93501f, 0.81721f,	0.0055f, 0.9998f, 0.0195f,
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 672, 84);
	meshListModel.push_back(holocron);

}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 5.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/star_wars_dado.png");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/holocron_tex.png");
	holocronTexture.LoadTextureA();
	
	Holocron_M = Model();
	Holocron_M.LoadModel("Models/holocron_tex.obj");
	
	Dado_M = Model();
	Dado_M.LoadModel("Models/dado_logos.obj");
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");

	skybox = Skybox(skyboxFaces);

	//Luces: ambiente fuerte + direccional + 3 puntuales (arriba-izq, atras-der, debajo/centro)
	//direccion = hacia donde viaja la luz
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.35f, 0.50f, 0.3f, -1.0f, -0.5f);
	pointLights[0] = PointLight(1.0f, 0.92f, 0.78f, 0.0f, 1.6f, -8.0f, 8.0f, -1.0f, 1.0f, 0.07f, 0.02f);
	pointLights[1] = PointLight(0.80f, 0.90f, 1.0f, 0.0f, 1.6f, 9.0f, 7.0f, -13.0f, 1.0f, 0.07f, 0.02f);
	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f, 0.0f, 0.9f, 0.0f, 2.5f, -6.0f, 1.0f, 0.07f, 0.02f);
	materialBrillante = Material(0.6f, 32.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		GLuint prog = shaderList[0].getShaderID();
		mainLight.UseLight(prog);
		for (int i = 0; i < 3; i++)
			pointLights[i].UseLight(prog, i);
		glUniform1i(glGetUniformLocation(prog, "pointLightCount"), 3);
		glUniform1i(glGetUniformLocation(prog, "spotLightCount"), 0);
		materialBrillante.UseMaterial(prog);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshListModel[2]->RenderMeshModel();


		

		//Dado texturizado por codigo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.5f, 4.5f, -6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//Dado importado desde Blender
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 4.5f, -6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();

		//Holocron texturizado por codigo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, 4.5f, -6.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();

		//Holocron importado desde Blender
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0f, 4.5f, -6.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Holocron_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <glew.h>
#include <glfw3.h>
#include <conio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

/******LIBRERÍA_AUDIO*********/
#include <irrKlang/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") //Inicia con irrKlang.dll

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetu = 0.0f;
float toffsetv = 0.0f;
float reproduciranimacion, habilitaranimacion,
guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

//Variable SkyBox
bool isDia;

//Variables animación compleja Pez
float	movPez_x = 0.0f,
movPez_y = 0.0f,
movPez_z = -330.0f,
orienta = 0.0f;

bool	recorrido1 = true, //Decidiendo la 1era que se quería emplear
recorrido2 = false,
recorrido3 = false,
recorrido4 = false,
recorrido5 = false;

bool animacionPez = false;
bool luz1 = false;
bool luz2 = false;

//Variables animación compleja Silla
float	movSilla_x = 0.0f;

bool	recorridoSilla1 = true,
recorridoSilla2 = false;

bool reproducirSilla = false;

//Animación globo
float	movGlobo_x = 0.0f,
movGlobo_z = -20.0f;

bool	recorridoGlobo1 = true,
recorridoGlobo2 = false,
recorridoGlobo3 = false,
recorridoGlobo4 = false;

bool animacionGlobo = false;
unsigned int spotLightCount = 0;

//Animación terraqueo
float	rotGloboTerra,
rotGloboTerraOffset,
movGloboTerraOffset;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Camera camera2;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture texcar;
Texture texwheel;
Texture FennekoText;
Texture RetsukoText;
Texture lamparaText;
Texture PezText;
Texture madera;
Texture Portada;
Texture tex_mesa;
Texture tex_patas;
Texture Edificio;
Texture texTerra;
Texture tex_cuadro;
Texture tex_cuadro2;
Texture marmol;

Texture tex_cafe1;
Texture tex_planta;
Texture tex_rama;
Texture tex_maceta;

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model Fenneko;
Model Pez;
Model silla;
Model Pecera;
Model papelPapel;
Model Terraqueo;
Model microondas;

Model llanta_AIz;
Model llanta_ADer;
Model llanta_DeIz;
Model llanta_DeDer;
Model carro_Chasis;
Model cofre;
Model Soporte;
Model basura;
Model puerta;
Model estufa;
Model laptop;
Model Retsuko;
Model fenne;
Model izq;
Model der;
Model Globo;

Model escritorio;
Model cuadro;
Model cuadro2;
Model cafetera;
Model taza;
Model maceta;
Model lampara;
Model lavabo;

Skybox skybox;

std::vector<std::string> skyboxFacesD;
std::vector<std::string> skyboxFacesN;

//materiales
Material Material_brillante;
Material Material_opaco;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

/**MÚSICA_2D****/
void musica2D();

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//PARA INPUT CON KEYFRAMES 
void inputKeyframes(bool* keys);

//Input luces

void inputLuces(bool* keys);


///////////////////////////////////////// Cambia solo el skybox
/*void cambioSkyBox(bool* keys) {
	if (keys[GLFW_KEY_1]) {
		isDia = !isDia;
		skybox = Skybox(isDia ? skyboxFacesD : skyboxFacesN); //Para cambiar de Skybox
		//Aquí añadir decremento de iluminación ambiental

		if (isDia == true) {
			//luz direccional, sólo 1 y siempre debe de existir
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.5f, 0.5f,
				0.0f, 0.0f, -1.0f);
		}
		else {
			//luz direccional, sólo 1 y siempre debe de existir
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.1f, 0.1f,
				0.0f, 0.0f, -1.0f);
		}
	}
}*/


//////////////////////////////////////// Cambia skybox y añade luces

// CAmbio de skybox con una tecla.
/*void cambioSkyBox(bool* keys) {
	static bool isDia = true; // Variable estática para conservar su valor entre llamadas

	//if (keys[GLFW_KEY_1]) {
	if (int(deltaTime) % 300 == 0)
		{
		if (isDia == true) {
			isDia = false;
		}
		else {
			isDia = false;
		}
		}
		isDia = !isDia;
		std::cout << "Cambiando isDia a " << (isDia ? "verdadero" : "falso") << std::endl;
		// Aquí añadir decremento de iluminación ambiental

		// Temporizador de 40 segundos
		//std::chrono::seconds duracionEspera(5);
		//std::this_thread::sleep_for(duracionEspera);

		// Restablecer isDia a falso después de 40 segundos
		isDia = false;
		std::cout << "Cambiando isDia a falso después de 40 segundos" << std::endl;
		// Aquí añadir incremento de iluminación ambiental

		if (isDia == true) {
			// luz direccional, sólo 1 y siempre debe de existir
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.5f, 0.5f,
				0.0f, 0.0f, -1.0f);
		}
		else {
			// luz direccional, sólo 1 y siempre debe de existir
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.1f, 0.1f,
				0.0f, 0.0f, -1.0f);
	}
}*/

//////////////////////////////// Cambio skybox automatico con luces
void cambioSkyBox(bool* keys) {
	static bool isDia = true; // Variable estática para conservar su valor entre llamadas
	static float elapsedTime = 0.0f; // Variable estática para el tiempo transcurrido

	// Obtener el tiempo transcurrido desde el último fotograma
	auto currentTime = std::chrono::high_resolution_clock::now();
	static auto previousTime = currentTime;
	std::chrono::duration<float> deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	elapsedTime += deltaTime.count();

	//////////////////////////////////////////////////////////////////////////////////////////////////// aqui se cambia el tiempo
	if (elapsedTime >= 60.0f) {
		elapsedTime = 0.0f;

		isDia = !isDia;
		skybox = Skybox(isDia ? skyboxFacesD : skyboxFacesN); //Para cambiar de Skybox
		std::cout << "Cambiando isDia a " << (isDia ? "verdadero" : "falso") << std::endl;

		// Aquí añadir decremento de iluminación ambiental
		// ...

		if (isDia) {
			// luz direccional para el modo día
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.5f, 0.5f,
				0.0f, -1.0f, 0.0f);
				spotLightCount--;
				spotLightCount--;
			
		}
		else {
			// luz direccional para el modo noche
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.05f, 0.1f,
				0.0f, 0.0f, -1.0f);

			//linterna
			spotLights[0] = SpotLight(0.7f, 0.9f, 1.0f,
				0.5f, 2.0f,
				0.0f, 10.0f, -55.0f,
				0.0f, -1.0f, -1.0f,
				1.0f, 0.0f, 0.0f,
				55.0f);
			spotLightCount++;

			//luz de lampara 
			//spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
			//	1.0f, 2.0f,
			//	30.4f, 7.6f, 0.0f,
			//	0.2f, -1.0f, 0.0f,
			//	1.0f, 0.0f, 0.0f,
			//	15.0f);
			//spotLightCount++;
						
			//luz de globo terraqueo
			spotLights[1] = SpotLight(0.3f, 0.7f, 0.8f,
				1.0f, 2.0f,
				-16.4f, 3.6f, 7.0f,
				0.2f, 1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				55.0f);
			spotLightCount++;

		}
	}
}


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

///////////////////////////////////// Inicia creación de primitivas


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


	unsigned int libroIndices[] = {
		// frente
		0, 1, 2,
		2, 3, 0,
		// derecha
		4, 5, 6,
		6, 7, 4,
		// atrás
		8, 9, 10,
		10, 11, 8,
		// izquierda
		12, 13, 14,
		14, 15, 12,
		// debajo
		16, 17, 18,
		18, 19, 16,
		// arriba
		20, 21, 22,
		22, 23, 20,
	};

	GLfloat libroVertices[] = {
		// frente
		//x                y                z                S                T                        NX                NY                NZ
		-0.5f, -0.5f,  0.5f,        0.75f,  0.34f,                0.0f,        0.0f,        -1.0f,        //0
		0.5f, -0.5f,  0.5f,                0.5f,        0.34f,                0.0f,        0.0f,        -1.0f,        //1
		0.5f,  0.5f,  0.5f,                0.5f,        0.6f,                0.0f,        0.0f,        -1.0f,        //2
		-0.5f,  0.5f,  0.5f,        0.75f,        0.6f,                0.0f,        0.0f,        -1.0f,        //3
		// derecha
		
		0.5f, -0.5f,  0.5f,            0.9f,  0.4f,                -1.0f,        0.0f,        0.0f,
		0.5f, -0.5f,  -0.5f,        0.75f,        0.4f,                -1.0f,        0.0f,        0.0f,
		0.5f,  0.5f,  -0.5f,        0.75f,        0.6f,                -1.0f,        0.0f,        0.0f,
		0.5f,  0.5f,  0.5f,            0.9f,        0.6f,                -1.0f,        0.0f,        0.0f,
		// atras
		-0.5f, -0.5f, -0.5f,        0.25f,  0.4f,                0.0f,        0.0f,        1.0f,
		0.5f, -0.5f, -0.5f,                0.1f,        0.4f,                0.0f,        0.0f,        1.0f,
		0.5f,  0.5f, -0.5f,                0.1f,        0.55f,                0.0f,        0.0f,        1.0f,
		-0.5f,  0.5f, -0.5f,        0.25f,        0.55f,                0.0f,        0.0f,        1.0f,
		// izquierda
		
		-0.5f, -0.5f,  -0.5f,        0.5f,  0.6f,                1.0f,        0.0f,        0.0f,
		-0.5f, -0.5f,  0.5f,        0.25f,        0.6f,                1.0f,        0.0f,        0.0f,
		-0.5f,  0.5f,  0.5f,        0.25f,        0.5f,                1.0f,        0.0f,        0.0f,
		-0.5f,  0.5f,  -0.5f,        0.5f,        0.5f,                1.0f,        0.0f,        0.0f,
		// debajo o fondo
	
		-0.5f, -0.5f,  0.5f,        0.74f,  0.33f,                0.0f,        1.0f,        0.0f,
		0.5f,  -0.5f,  0.5f,        0.5f,        0.33f,                0.0f,        1.0f,        0.0f,
		 0.5f,  -0.5f,  -0.5f,        0.5f,        0.1f,                0.0f,        1.0f,        0.0f,
		-0.5f, -0.5f,  -0.5f,        0.74f,        0.1f,                0.0f,        1.0f,        0.0f,
		//arriba
		
		 -0.5f, 0.5f,  0.5f,        0.7f,  0.7f,                0.0f,        -1.0f,        0.0f,
		 0.5f,  0.5f,  0.5f,        0.7f,        0.9f,                0.0f,        -1.0f,        0.0f,
		  0.5f, 0.5f,  -0.5f,        0.55f,        0.9f,                0.0f,        -1.0f,        0.0f,
		 -0.5f, 0.5f,  -0.5f,        0.55f,        0.7f,                0.0f,        -1.0f,        0.0f,
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


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(libroVertices, libroIndices, 192, 36);
	meshList.push_back(obj6);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



//Animación silla - (Básica)
void animateSilla(bool* keys) { 
	if (keys[GLFW_KEY_4]) {
		reproducirSilla = !reproducirSilla;
	}
	if (reproducirSilla) {
		if (recorridoSilla1) {
			movSilla_x += 0.2f;
			if (movSilla_x >= 10.0f) {
				recorridoSilla1 = false;
				recorridoSilla2 = true;
			}
		}
		if (recorridoSilla2) {
			movSilla_x -= 0.2f;
			if (movSilla_x <= -5.0f) {
				recorridoSilla2 = false;
				recorridoSilla1 = true;
				reproducirSilla = !reproducirSilla;
			}
		}
	}
}



/*/Animación taza - (Básica)
void animateSilla(bool* keys) {
	if (keys[GLFW_KEY_5]) {
		reproducirTaza = !reproducirTaza;
	}
	if (reproducirTaza) {
		if (vueltaTaza) 
			movTaza += 0.2f;
			
		if (recorridoSilla2) {
			movSilla_x -= 0.2f;
			if (movSilla_x <= -5.0f) {
				recorridoSilla2 = false;
				recorridoSilla1 = true;
				reproducirSilla = !reproducirSilla;
			}
		}
	}
}*/

//NEW// Keyframes
float posXpapel = -15.0, posYpapel = -0.0, posZpapel = 0;
float	movpapel_x = 0.0f, movpapel_y = 0.0f, movpapel_z = 0.0f;
float giropapel = 0;

#define MAX_FRAMES 50
int i_max_steps = 270;
int i_curr_steps = 9;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movpapel_x;		//Variable para PosicionX
	float movpapel_y;		//Variable para PosicionY
	float movpapel_z;		//Variable para PosicionZ
	float movpapel_xInc;		//Variable para IncrementoX
	float movpapel_yInc;		//Variable para IncrementoY
	float movpapel_zInc;		//Variable para IncrementoZ
	float giropapel;
	float giropapelInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 46;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) {

	printf("frameindex %d\n", FrameIndex);
	KeyFrame[FrameIndex].movpapel_x = movpapel_x;
	printf("KeyFrame[FrameIndex].X = Valor; %d\n", KeyFrame[FrameIndex].movpapel_x);
	KeyFrame[FrameIndex].movpapel_y = movpapel_y;
	printf("KeyFrame[FrameIndex].Y = Valor; %d\n", KeyFrame[FrameIndex].movpapel_y);
	KeyFrame[FrameIndex].movpapel_z = movpapel_z;
	printf("KeyFrame[FrameIndex].Z = Valor; %d\n", KeyFrame[FrameIndex].movpapel_z);
	KeyFrame[FrameIndex].giropapel = giropapel;
	printf("KeyFrame[FrameIndex].giropapel = Valor; %d\n", KeyFrame[FrameIndex].giropapel);
	FrameIndex++;
}

void resetElements(void) {

	movpapel_x = KeyFrame[0].movpapel_x;
	movpapel_y = KeyFrame[0].movpapel_y;
	movpapel_y = KeyFrame[0].movpapel_z;
	giropapel = KeyFrame[0].giropapel;
}

void interpolation(void) {
	KeyFrame[playIndex].movpapel_xInc = (KeyFrame[playIndex + 1].movpapel_x - KeyFrame[playIndex].movpapel_x) / i_max_steps;
	KeyFrame[playIndex].movpapel_yInc = (KeyFrame[playIndex + 1].movpapel_y - KeyFrame[playIndex].movpapel_y) / i_max_steps;
	KeyFrame[playIndex].movpapel_zInc = (KeyFrame[playIndex + 1].movpapel_z - KeyFrame[playIndex].movpapel_z) / i_max_steps;
	KeyFrame[playIndex].giropapelInc = (KeyFrame[playIndex + 1].giropapel - KeyFrame[playIndex].giropapel) / i_max_steps;

}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("Frame [%d] reproducido \n", playIndex - 1);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Ultimo frame es [%d] \n", FrameIndex - 1);
				//fprintf(guardarFrames,"FrameIndex [%d]" ,FrameIndex);
				printf("Termina animaci�n\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aqu�\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se qued� aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movpapel_x += KeyFrame[playIndex].movpapel_xInc;
			movpapel_y += KeyFrame[playIndex].movpapel_yInc;
			movpapel_z += KeyFrame[playIndex].movpapel_zInc;
			giropapel += KeyFrame[playIndex].giropapelInc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);
	camera2 = Camera(glm::vec3(20.0f, 30.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -120.0f, -30.0f, 25.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/PisoMadera.png");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");


	
	basura = Model();
	basura.LoadModel("Models/basura.fbx");

	lavabo = Model();
	lavabo.LoadModel("Models/sink.fbx");

	laptop = Model();
	laptop.LoadModel("Models/laptop.fbx");

	estufa = Model();
	estufa.LoadModel("Models/estufa.fbx");
	tex_mesa = Texture("Textures/tex_mesa.png");
	tex_mesa.LoadTextureA();
	tex_patas = Texture("Textures/tex_patas.png");
	tex_patas.LoadTextureA();

	Portada = Texture("Textures/portada.png");
	Portada.LoadTextureA();
	madera = Texture("Textures/madera.png");
	madera.LoadTextureA();

	marmol = Texture("Textures/marmol.jpg");
	marmol.LoadTextureA();

	Edificio = Texture("Textures/edificio.png");
	Edificio.LoadTextureA();

	texcar = Texture("Textures/tex_car.tga");
	texcar.LoadTextureA();
	texwheel = Texture("Textures/tex_wheel.tga");
	texwheel.LoadTextureA();

	//modelos cortados
	fenne = Model();
	fenne.LoadModel("Models/Fenneko/cuerpoFenneko.fbx");

	izq = Model();
	izq.LoadModel("Models/Fenneko/bIzqFenneko.fbx");

	der = Model();
	der.LoadModel("Models/Fenneko/bDerFenneko.fbx");
	// acaban modelos cortados
	FennekoText = Texture("Textures/Fenneko_Color.png");
	FennekoText.LoadTextureA();

	RetsukoText = Texture("Textures/Aggretsukko_Color.png");
	RetsukoText.LoadTextureA();

	//Modelo pecera
	Pecera = Model();
	Pecera.LoadModel("Models/pecera2.fbx");


	//Globo terraqueo 
	Terraqueo = Model();
	Terraqueo.LoadModel("Models/Globo-Terraqueo-Mapa-Fisico.fbx");

	//Modelo soporte globo terraqueo
	Soporte = Model();
	Soporte.LoadModel("Models/Vase32.fbx");

	texTerra = Texture("Textures/2209.jpg");
	texTerra.LoadTextureA();

	//Modelo silla
	silla = Model();
	silla.LoadModel("Models/Office_chair.obj");

	//Modelo escritorio
	escritorio = Model();
	escritorio.LoadModel("Models/escritorio.fbx");

	//Modelo cuadro
	cuadro = Model();
	cuadro.LoadModel("Models/cuadro.fbx");
	tex_cuadro = Texture("Textures/kanagawa_oki_nami_ura_baseColor.png");
	tex_cuadro.LoadTexture();
	//Modelo cuadro2
	cuadro2 = Model();
	cuadro2.LoadModel("Models/cuadro2.fbx");
	tex_cuadro2 = Texture("Textures/material_0_baseColor.png");
	tex_cuadro2.LoadTexture();

	//Modelo cafetera
	cafetera = Model();
	cafetera.LoadModel("Models/cafetera.fbx");
	tex_cafe1 = Texture("Textures/machin_baseColor.png");
	tex_cafe1.LoadTexture();
	//Modelo taza
	taza = Model();
	taza.LoadModel("Models/taza.fbx");

	//Modelo maceta
	maceta = Model();
	maceta.LoadModel("Models/maceta.fbx");
	tex_planta = Texture("Textures/Material__26_baseColor.png");
	tex_planta.LoadTexture();
	tex_rama = Texture("Textures/Material__27_baseColor.png");
	tex_rama.LoadTexture();
	tex_maceta = Texture("Textures/Material__25_baseColor.png");
	tex_maceta.LoadTexture();

	microondas = Model();
	microondas.LoadModel("Models/microondas.fbx");

	carro_Chasis = Model();
	carro_Chasis.LoadModel("Models/carro_chasis.obj");

	cofre = Model();
	cofre.LoadModel("Models/cofre.obj");

	llanta_ADer = Model();
	llanta_ADer.LoadModel("Models/llantaADer.obj");

	llanta_AIz = Model();
	llanta_AIz.LoadModel("Models/llantaAIz.obj");

	llanta_DeIz = Model();
	llanta_DeIz.LoadModel("Models/llantaDeIz.obj");

	llanta_DeDer = Model();
	llanta_DeDer.LoadModel("Models/llantaDeDer.obj");

	Fenneko = Model();
	Fenneko.LoadModel("Models/Fenneko.fbx");

	Retsuko = Model();
	Retsuko.LoadModel("Models/Retsuko.fbx");

	//Modelo y textura de lampara
	lampara = Model();
	lampara.LoadModel("Models/desk_lamp.obj");
	lamparaText = Texture("Textures/lamp_Albedo.png");
	lamparaText.LoadTextureA();

	//Modelo y textura de tiburon
	Pez = Model();
	Pez.LoadModel("Models/Leopard_Shark.fbx");
	PezText = Texture("Textures/Material1.png");
	PezText.LoadTextureA();

	//Modelo Globo Aeroestático
	Globo = Model();
	Globo.LoadModel("Models/Globo.fbx");

	//Modelo avión de papel
	papelPapel = Model();
	papelPapel.LoadModel("Models/paper_airplane.obj");

	//Skybox Día
	skyboxFacesD.push_back("Textures/Skybox/Cielo-D/miramar_rt.tga");
	skyboxFacesD.push_back("Textures/Skybox/Cielo-D/miramar_lf.tga");
	skyboxFacesD.push_back("Textures/Skybox/Cielo-D/miramar_dn.tga");
	skyboxFacesD.push_back("Textures/Skybox/Cielo-D/miramar_up.tga");
	skyboxFacesD.push_back("Textures/Skybox/Cielo-D/miramar_bk.tga");
	skyboxFacesD.push_back("Textures/Skybox/Cielo-D/miramar_ft.tga");

	//Skybox Noche
	skyboxFacesN.push_back("Textures/Skybox/Cielo-N/1.png");
	skyboxFacesN.push_back("Textures/Skybox/Cielo-N/3.png");
	skyboxFacesN.push_back("Textures/Skybox/Cielo-N/6.png");
	skyboxFacesN.push_back("Textures/Skybox/Cielo-N/5.png");
	skyboxFacesN.push_back("Textures/Skybox/Cielo-N/2.png");
	skyboxFacesN.push_back("Textures/Skybox/Cielo-N/4.png");

	skybox = Skybox(skyboxFacesD);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);




	////////////////////////////////////////////////////////////////////////////////////// Luces con encendido y apagado con botón

	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	/*pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;*/




	//////////////////////////////////////////////////////////////////////////////////////////////////////// Acaban luces 
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.09f;
	avanza = true;

	//Variables globo terraqueo
	movGloboTerraOffset = 0.9f;
	rotGloboTerra = 0.0f;
	rotGloboTerraOffset = 0.9f;

	musica2D();

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		animacionPez = !animacionPez;
		if (animacionPez) {
			if (recorrido1) {
				movPez_y += 0.2f;
				orienta = 0.0f;
				if (movPez_y >= 10.0f) {
					recorrido1 = false;
					recorrido2 = true;
				}
			}
			if (recorrido2) {
				movPez_x -= 0.2f;
				orienta = 0.0f;
				if (movPez_x <= -10.0f) {
					recorrido2 = false;
					recorrido3 = true;
				}
			}
			if (recorrido3) {
				movPez_y += 0.2f;
				orienta = 180.0f;
				if (movPez_y >= 15.0f) {
					recorrido3 = false;
					recorrido4 = true;
				}
			}
			if (recorrido4) {
				movPez_x += 0.2f;
				orienta = 180.0f;
				if (movPez_x >= 5.0f) {
					recorrido4 = false;
					recorrido5 = true;
				}
			}
			if (recorrido5) {
				movPez_y -= 0.2f;
				orienta = 0.0f;
				if (movPez_y <= 0.0f) {
					recorrido5 = false;
					recorrido1 = true;
				}
			}
		}

		animacionGlobo = !animacionGlobo;
		if (animacionGlobo) {
			if (recorridoGlobo1) {
				movGlobo_z += 0.2f;
				if (movGlobo_z >= 20.0f) {
					recorridoGlobo1 = false;
					recorridoGlobo2 = true;
				}
			}
			if (recorridoGlobo2) {
				movGlobo_x -= 0.2f;
				if (movGlobo_x <= -20.0f) {
					recorridoGlobo2 = false;
					recorridoGlobo3 = true;
				}
			}
			if (recorridoGlobo3) {
				movGlobo_z -= 0.2f;
				if (movGlobo_z <= -20.0f) {
					recorridoGlobo3 = false;
					recorridoGlobo4 = true;
				}
			}
			if (recorridoGlobo4) {
				movGlobo_x += 0.2f;
				if (movGlobo_x >= 0.0f) {
					recorridoGlobo4 = false;
					recorridoGlobo1 = true;
				}
			}
		}


		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Animación giro del Globo Terraqueo
		rotGloboTerra += rotGloboTerraOffset * deltaTime;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		cambioSkyBox(mainWindow.getsKeys());
		animateSilla(mainWindow.getsKeys());
		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		glm::vec3 posAvionPapel = glm::vec3(2.0f, 0.0f, 0.0f);



		//KEYFRAMES DECLARADOS INICIALES

		KeyFrame[0].movpapel_x = 0.0f;
		KeyFrame[0].movpapel_y = 0.0f;
		KeyFrame[0].movpapel_z = 0.0f;
		KeyFrame[0].giropapel = 0;


		KeyFrame[1].movpapel_x = 0.0f;
		KeyFrame[1].movpapel_y = 10.0f;
		KeyFrame[1].movpapel_z = 0.0f;
		KeyFrame[1].giropapel = 0;


		KeyFrame[2].movpapel_x = 5.0f;
		KeyFrame[2].movpapel_y = 6.0f;
		KeyFrame[2].movpapel_z = 0.0f;
		KeyFrame[2].giropapel = 0;


		KeyFrame[3].movpapel_x = 7.0f;
		KeyFrame[3].movpapel_y = 10.0f;
		KeyFrame[3].movpapel_z = 0.0f;
		KeyFrame[3].giropapel = 0;

		KeyFrame[4].movpapel_x = 9.0f;
		KeyFrame[4].movpapel_y = 6.0f;
		KeyFrame[4].movpapel_z = 0.0f;
		KeyFrame[4].giropapel = 0.0f;

		//Agregar Kefyrame[5] para que el avi�n regrese al inicio

		KeyFrame[5].movpapel_x = 11.0f;
		KeyFrame[5].movpapel_y = 10.0f;
		KeyFrame[5].movpapel_z = 0.0f;
		KeyFrame[5].giropapel = 0;

		KeyFrame[6].movpapel_x = 13.0f;
		KeyFrame[6].movpapel_y = 6.0f;
		KeyFrame[6].movpapel_z = 0.0f;
		KeyFrame[6].giropapel = 0;

		KeyFrame[7].movpapel_x = 15.0f;
		KeyFrame[7].movpapel_y = 10.0f;
		KeyFrame[7].movpapel_z = 0.0f;
		KeyFrame[7].giropapel = 0;

		KeyFrame[8].movpapel_x = 17.0f;
		KeyFrame[8].movpapel_y = 6.0f;
		KeyFrame[8].movpapel_z = 0.0f;
		KeyFrame[8].giropapel = 90;

		KeyFrame[9].movpapel_x = 19.0f;
		KeyFrame[9].movpapel_y = 10.0f;
		KeyFrame[9].movpapel_z = -4.0f;
		KeyFrame[9].giropapel = 90;

		KeyFrame[10].movpapel_x = 19.0f;
		KeyFrame[10].movpapel_y = 6.0f;
		KeyFrame[10].movpapel_z = -8.0f;
		KeyFrame[10].giropapel = 90;

		KeyFrame[11].movpapel_x = 19.0f;
		KeyFrame[11].movpapel_y = 10.0f;
		KeyFrame[11].movpapel_z = -12.0f;
		KeyFrame[11].giropapel = 90;

		KeyFrame[12].movpapel_x = 19.0f;
		KeyFrame[12].movpapel_y = 6.0f;
		KeyFrame[12].movpapel_z = -16.0f;
		KeyFrame[12].giropapel = 90;

		KeyFrame[13].movpapel_x = 19.0f;
		KeyFrame[13].movpapel_y = 10.0f;
		KeyFrame[13].movpapel_z = -20.0f;
		KeyFrame[13].giropapel = 90;

		KeyFrame[14].movpapel_x = 19.0f;
		KeyFrame[14].movpapel_y = 6.0f;
		KeyFrame[14].movpapel_z = -24.0f;
		KeyFrame[14].giropapel = 90;

		KeyFrame[15].movpapel_x = 19.0f;
		KeyFrame[15].movpapel_y = 10.0f;
		KeyFrame[15].movpapel_z = -28.0f;
		KeyFrame[15].giropapel = 90;

		KeyFrame[16].movpapel_x = 19.0f;
		KeyFrame[16].movpapel_y = 6.0f;
		KeyFrame[16].movpapel_z = -32.0f;
		KeyFrame[16].giropapel = 90;

		KeyFrame[17].movpapel_x = 19.0f;
		KeyFrame[17].movpapel_y = 10.0f;
		KeyFrame[17].movpapel_z = -36.0f;
		KeyFrame[17].giropapel = 90;

		KeyFrame[18].movpapel_x = 19.0f;
		KeyFrame[18].movpapel_y = 6.0f;
		KeyFrame[18].movpapel_z = -40.0f;
		KeyFrame[18].giropapel = 90;

		KeyFrame[19].movpapel_x = 19.0f;
		KeyFrame[19].movpapel_y = 5.0f;
		KeyFrame[19].movpapel_z = -44.0f;
		KeyFrame[19].giropapel = 90;

		KeyFrame[20].movpapel_x = 19.0f;
		KeyFrame[20].movpapel_y = 4.0f;
		KeyFrame[20].movpapel_z = -48.0f;
		KeyFrame[20].giropapel = 90;

		KeyFrame[21].movpapel_x = 19.0f;
		KeyFrame[21].movpapel_y = 3.0f;
		KeyFrame[21].movpapel_z = -52.0f;
		KeyFrame[21].giropapel = 90;

		KeyFrame[22].movpapel_x = 19.0f;
		KeyFrame[22].movpapel_y = 2.0f;
		KeyFrame[22].movpapel_z = -56.0f;
		KeyFrame[22].giropapel = 90;

		KeyFrame[23].movpapel_x = 19.0f;
		KeyFrame[23].movpapel_y = 1.0f;
		KeyFrame[23].movpapel_z = -60.0f;
		KeyFrame[23].giropapel = 90;

		KeyFrame[24].movpapel_x = 19.0f;
		KeyFrame[24].movpapel_y = 0.0f;
		KeyFrame[24].movpapel_z = -64.0f;
		KeyFrame[24].giropapel = 90;

		//Aqui llega al escenario y esta en el suelo
		KeyFrame[25].movpapel_x = 19.0f;
		KeyFrame[25].movpapel_y = 0.0f;
		KeyFrame[25].movpapel_z = -68.0f;
		KeyFrame[25].giropapel = 90;
		//Aqui esta en el suelo y gira
		KeyFrame[26].movpapel_x = 19.0f;
		KeyFrame[26].movpapel_y = 0.0f;
		KeyFrame[26].movpapel_z = -68.0f;
		KeyFrame[26].giropapel = 180;
		//Comienza a elevarse
		KeyFrame[27].movpapel_x = 15.0f;
		KeyFrame[27].movpapel_y = 1.0f;
		KeyFrame[27].movpapel_z = -68.0f;
		KeyFrame[27].giropapel = 180;

		KeyFrame[28].movpapel_x = 11.0f;
		KeyFrame[28].movpapel_y = 2.f;
		KeyFrame[28].movpapel_z = -68.0f;
		KeyFrame[28].giropapel = 180;

		KeyFrame[29].movpapel_x = 7.0f;
		KeyFrame[29].movpapel_y = 3.0f;
		KeyFrame[29].movpapel_z = -68.0f;
		KeyFrame[29].giropapel = 180;

		KeyFrame[30].movpapel_x = 3.0f;
		KeyFrame[30].movpapel_y = 4.0f;
		KeyFrame[30].movpapel_z = -68.0f;
		KeyFrame[30].giropapel = 180;

		KeyFrame[31].movpapel_x = -1.0f;
		KeyFrame[31].movpapel_y = 5.0f;
		KeyFrame[31].movpapel_z = -68.0f;
		KeyFrame[31].giropapel = 180;

		KeyFrame[32].movpapel_x = -5.0f;
		KeyFrame[32].movpapel_y = 6.0f;
		KeyFrame[32].movpapel_z = -68.0f;
		KeyFrame[32].giropapel = 180;
		//Gira de nuevo
		KeyFrame[33].movpapel_x = -5.0f;
		KeyFrame[33].movpapel_y = 6.0f;
		KeyFrame[33].movpapel_z = -68.0f;
		KeyFrame[33].giropapel = 270;
		//Se desplaza para regresar a punto inicial
		KeyFrame[34].movpapel_x = -5.0f;
		KeyFrame[34].movpapel_y = 10.0f;
		KeyFrame[34].movpapel_z = -60.0f;
		KeyFrame[34].giropapel = 270;

		KeyFrame[35].movpapel_x = -5.0f;
		KeyFrame[35].movpapel_y = 6.0f;
		KeyFrame[35].movpapel_z = -52.0f;
		KeyFrame[35].giropapel = 270;

		KeyFrame[36].movpapel_x = -5.0f;
		KeyFrame[36].movpapel_y = 10.0f;
		KeyFrame[36].movpapel_z = -44.0f;
		KeyFrame[36].giropapel = 270;

		KeyFrame[37].movpapel_x = -5.0f;
		KeyFrame[37].movpapel_y = 6.0f;
		KeyFrame[37].movpapel_z = -36.0f;
		KeyFrame[37].giropapel = 270;

		KeyFrame[38].movpapel_x = -5.0f;
		KeyFrame[38].movpapel_y = 10.0f;
		KeyFrame[38].movpapel_z = -28.0f;
		KeyFrame[38].giropapel = 270;

		KeyFrame[39].movpapel_x = -5.0f;
		KeyFrame[39].movpapel_y = 6.0f;
		KeyFrame[39].movpapel_z = -20.0f;
		KeyFrame[39].giropapel = 270;

		KeyFrame[40].movpapel_x = -5.0f;
		KeyFrame[40].movpapel_y = 10.0f;
		KeyFrame[40].movpapel_z = -12.0f;
		KeyFrame[40].giropapel = 270;

		KeyFrame[41].movpapel_x = -5.0f;
		KeyFrame[41].movpapel_y = 6.0f;
		KeyFrame[41].movpapel_z = -4.0f;
		KeyFrame[41].giropapel = 270;

		KeyFrame[42].movpapel_x = -5.0f;
		KeyFrame[42].movpapel_y = 5.0f;
		KeyFrame[42].movpapel_z = 0.0f;
		KeyFrame[42].giropapel = 270;
		//Gira 
		KeyFrame[43].movpapel_x = -5.0f;
		KeyFrame[43].movpapel_y = 5.0f;
		KeyFrame[43].movpapel_z = 0.0f;
		KeyFrame[43].giropapel = 360;
		//Desciende
		KeyFrame[44].movpapel_x = -5.0f;
		KeyFrame[44].movpapel_y = 0.0f;
		KeyFrame[44].movpapel_z = 0.0f;
		KeyFrame[44].giropapel = 360;
		//Termina ciclo de vuelo
		KeyFrame[45].movpapel_x = 0.0f;
		KeyFrame[45].movpapel_y = 0.0f;
		KeyFrame[45].movpapel_z = 0.0f;
		KeyFrame[45].giropapel = 360;

		/*****Se tiene implementado el cambio de cámara*************/
		if (mainWindow.getactivaCamara()) { //La tecla N para la cámara isométrica
			camera2.keyControl(mainWindow.getsKeys(), deltaTime * 0.5f);
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera2.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera2.getCameraPosition().x, camera2.getCameraPosition().y, camera2.getCameraPosition().z);
		}
		else {//La tecla M para la cámara normal
			camera.keyControl(mainWindow.getsKeys(), deltaTime * 5);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		}
		/********************************/


		//Globo Aeroestático
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movGlobo_x, 50.0f, movGlobo_z));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Globo.RenderModel();

		//Globo Terráqueo 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-16.0f, 12.0f, 7.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotGloboTerra * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		texTerra.UseTexture();
		Terraqueo.RenderModel();

		//Soporte globo terraqueo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-17.1f, 3.4f, 7.8f));
		model = glm::scale(model, glm::vec3(0.18f, 0.07f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tex_patas.UseTexture();
		Soporte.RenderModel();


		//Sillas
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(movSilla_x, 0.0f, -10.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -55 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getACCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		//Tiburon
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::translate(model, glm::vec3(movPez_x, movPez_y, movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PezText.UseTexture();
		Pez.RenderModel();

		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -102.0f, 100.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		//Libros y librero
		Edificio.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -102.0f, -100.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Edificio.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -102.0f, 00.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Edificio.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -102.0f, 00.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Edificio.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		//////////////////////////////////////////////////////////////////////Libros y librero

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.8f, 8.0f, -30.0f));
		model = glm::scale(model, glm::vec3(10.0f, 3.0f, 35.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		madera.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.8f, 12.5f, -25.0f));
		model = glm::scale(model, glm::vec3(10.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Portada.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.8f, 12.5f, -27.3f));
		model = glm::scale(model, glm::vec3(10.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Portada.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.8f, 12.5f, -29.6f));
		model = glm::scale(model, glm::vec3(10.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Portada.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.8f, 12.5f, -31.8f));
		model = glm::scale(model, glm::vec3(10.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Portada.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.8f, 12.5f, -34.0f));
		model = glm::scale(model, glm::vec3(10.0f, 6.0f, 2.0f));
		//model = glm::rotate(model, -55 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Portada.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.8f, 12.5f, -36.2f));
		model = glm::scale(model, glm::vec3(10.0f, 6.0f, 2.0f));
		//model = glm::rotate(model, -55 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Portada.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.8f, 12.5f, -38.4f));
		model = glm::scale(model, glm::vec3(10.0f, 6.0f, 2.0f));
		//model = glm::rotate(model, -55 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Portada.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		////////////////////////////////////////////// Acaba librero y libros 


		///////////////////////////////////////////// Mesas

		/* Mesa - Sala 1*/

		/*Mesa 1*/ /*Tabla*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, 4.5f, -40.0f));
		model = glm::scale(model, glm::vec3(35.0f, 1.0f, 55.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_mesa.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		/*Mesa 1*/ /*Patas*/

		/*Izquierda atrás*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.8f, -0.5f, -15.7f));
		model = glm::scale(model, glm::vec3(5.0f, 9.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		/*Derecha atrás*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.8f, -0.5f, -63.7f));
		model = glm::scale(model, glm::vec3(5.0f, 9.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		/*Derecha adelante*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-49.8f, -0.5f, -63.7f));
		model = glm::scale(model, glm::vec3(5.0f, 9.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		/*Derecha izquierda*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-49.8f, -0.5f, -15.7f));
		model = glm::scale(model, glm::vec3(5.0f, 9.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Sillas

		//Silla 1
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(-80.8f, -1.5f, -25.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -235 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getACCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		//Silla 2
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(-80.8f, -1.5f, -50.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -285 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getACCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		//Silla 3
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(-35.8f, -1.5f, -50.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getACCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		//Silla 4
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(-35.8f, -1.5f, -25.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -95 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getACCofre()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();


		////////////////////////Laptops 

		//Laptop atras
		model = glm::mat4(1.0);
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(55.0f, 5.7f, -53.0));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		laptop.RenderModel();

		//Laptop medio centro
		model = glm::mat4(1.0);
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(30.0f, 5.7f, 8.5f));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		laptop.RenderModel();

		//Laptop medios derecha
		model = glm::mat4(1.0);
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(85.0f, 5.7f, 8.5f));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		laptop.RenderModel();

		//Laptop abajo derecha d
		model = glm::mat4(1.0);
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(68.0f, 5.7f, 70.0));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		laptop.RenderModel();

		//Laptop abajo izquierda
		model = glm::mat4(1.0);
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(45.0f, 5.7f, 70.0));
		model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		laptop.RenderModel();


		/*FIN DE MESA*/

	/* Mesa - Sala 2 (Horno y cafetera)*/

		/*Mesa 2*/ /*Tabla*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 4.1f, 7.5));
		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_mesa.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Mesa 2*/ /*Patas*/

		/*Izquierda atrás*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.9f, 2.5f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Derecha atrás*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.8f, 0.9f, 2.5f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Derecha adelante*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.8f, 0.9f, 13.0f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Derecha izquierda*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.9f, 13.0f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*FIN DE MESA*/

		/* Mesa - Sala 3 (Pecera pequeña)*/

		/*Mesa 3*/ /*Tabla*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-17.0f, 4.1f, 7.5));
		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_mesa.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Mesa 3*/ /*Patas*/

		/*Izquierda atrás*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.5f, 0.9f, 3.5f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Derecha atrás*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, 0.9f, 3.5f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Izquierda adelante*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.5f, 0.9f, 12.5f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Derecha adelante*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.8f, 0.9f, 12.5f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*FIN DE MESA*/

		///* Mesa 4 - Sala principal (Impresora, lapices)*/

		///*Mesa 4 */ /*Tabla*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.2f, 4.1f, -67.5f));
		model = glm::scale(model, glm::vec3(15.0f, 1.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_mesa.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Mesa 4 */ /*Patas*/

		///*Derecha atrás*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.2f, 0.7f, -69.5f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Izquierda atrás*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.5f, 0.7f, -69.5f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Derecha adelante*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.2f, 0.7f, -63.7f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*Derecha izquierda*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.5f, 0.7f, -63.7f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		tex_patas.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		///*FIN DE MESA*/


		////////////////////////////////////////////////////////////////Avión de papel KeyFrames

		posAvionPapel = glm::vec3(posXpapel + movpapel_x, posYpapel + movpapel_y, posZpapel + 15);
		model = glm::translate(model, posAvionPapel);
		model = glm::scale(model, glm::vec3(80.0f, 80.0f, 80.0f));
		model = glm::rotate(model, giropapel * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		papelPapel.RenderModel();

		///////////////////////////////////////////////////////// Termina papel KeyFrames

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 4.8f, 5.5f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		microondas.RenderModel();

		//cafetera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 7.5f, 5.5f));
		model = glm::scale(model, glm::vec3(2.8f, 2.8f, 2.8f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tex_cafe1.UseTexture();
		cafetera.RenderModel();

		//tazas de café de la cocina 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 4.8f, 9.5f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		taza.RenderModel();

		//tazas de café de la cocina 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 4.8f, 11.5f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		taza.RenderModel();

		//tazas de café de la cocina 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 4.8f, 13.5f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		taza.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-84.0f, 0.7f, 3.0));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 25.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tex_patas.UseTexture();
		basura.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-91.0f, 0.7f, 3.0));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 25.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		basura.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-97.0f, 0.7f, 3.0));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 25.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		basura.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, 8.7f, 60.0f));
		model = glm::scale(model, glm::vec3(6.0f, 7.0f, 10.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tex_patas.UseTexture();
		lavabo.RenderModel();

		//Escritorio 1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.1f, 0.75f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(350.0f, 0.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marmol.UseTexture();
		escritorio.RenderModel();

		//Escritorio2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.1f, 0.75f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(800.0f, -4.0f, -2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marmol.UseTexture();
		escritorio.RenderModel();

		//Escritorio 3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(3.0f, 0.75f, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(13.0f, -650.0f, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marmol.UseTexture();
		escritorio.RenderModel();

		//Escritorio 4
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(3.0f, 0.75f, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(20.0f, -650.0f, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marmol.UseTexture();
		escritorio.RenderModel();

		//Escritorio 5
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(3.0f, 0.75f, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(22.0f, 500.0f, -2.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marmol.UseTexture();
		escritorio.RenderModel();

		//Silla 5
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(40.0f, -1.5f, 9.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -95 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		//Silla 6
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(75.8f, -1.5f, 3.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -285 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		//Silla 7
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(55.0f, -1.5f, -43.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -185 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		//Silla 8
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(48.0f, -1.5f, 60.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -5 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		//Silla 9
		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(72.0f, -1.5f, 60.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, -75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		silla.RenderModel();

		//cuadro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-62.0f, 10.0f, -74.85f));
		model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tex_cuadro.UseTexture();
		cuadro.RenderModel();

		//cuadro2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 4.0f, -74.85f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tex_cuadro2.UseTexture();
		cuadro2.RenderModel();

		//taza 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f, 5.0f, -45.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		taza.RenderModel();

		//taza 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-73.0f, 5.0f, -30.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		taza.RenderModel();

		//taza 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-49.0f, 5.0f, -49.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		taza.RenderModel();

		//taza 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 5.0f, -33.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		taza.RenderModel();

		//Maceta 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, -2.0f, 80.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.11f, 0.1f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tex_planta.UseTexture();
		tex_rama.UseTexture();
		tex_maceta.UseTexture();
		maceta.RenderModel();

		//Maceta 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, -2.0f, -70.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.11f, 0.1f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tex_planta.UseTexture();
		tex_rama.UseTexture();
		tex_maceta.UseTexture();
		maceta.RenderModel();

		//Maceta 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -2.0f, 70.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.11f, 0.1f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tex_planta.UseTexture();
		tex_rama.UseTexture();
		tex_maceta.UseTexture();
		maceta.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-93.0f, -1.5f, 30.0));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.25f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estufa.RenderModel();


		///////////////////////////////////////////////////////////////////////////////////////////Paredes

		//Borde izquierdo completo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 3.0f, -1.0));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 150.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marmol.UseTexture();
		meshList[4]->RenderMesh();


		//Borde de atras completo

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -75.0));
		model = glm::scale(model, glm::vec3(200.0f, 30.0f, 150.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();


		//Pared de en medio perpendicular a la camara, apunta a X

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-62.5f, 3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(75.0f, 30.0f, 1.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-62.5f, 3.0f, -0.01f));
		model = glm::scale(model, glm::vec3(75.0f, 30.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		//Pared central, apunta a Z borde de abajo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 3.0f, 60.0));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 25.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.01f, 3.0f, 60.0));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 25.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		//Pared central, apunta a Z borde de medio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 3.0f, 5.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 45.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.01f, 3.0f, 5.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 45.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		//Pared central, apunta a Z borde de arriba
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 3.0f, -60.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.01f, 3.0f, -60.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		/////////////////////////////////////////////////////////////////// Terminan paredes

		////////////////////////////////////////////////////////////////// Puerta

		model = glm::mat4(1.0);
		modelaux = model;
		color = glm::vec3(0.87f, 0.35f, 1.0f);
		model = glm::translate(model, glm::vec3(-80.8f, -10.5f, -25.0));
		model = glm::scale(model, glm::vec3(60.2f, 60.2f, 60.2f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puerta.RenderModel();

		/////////////////////////////////////////////////////////////////////////////////////////Personajes

		//Retsuko
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -2.0f, 60.0));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RetsukoText.UseTexture();
		Retsuko.RenderModel();


		/////////////////////////////////////////////////Personajes cortados 
		
		//Fenneko
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -2.0f, 60.0));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FennekoText.UseTexture();
		fenne.RenderModel();

		//Fenneko brazo izquierdo
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.00f, -0.01f, 0.55));
		model = glm::rotate(model, movPez_x * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, movPez_x * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.3f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		modelaux = model;
		FennekoText.UseTexture();
		izq.RenderModel();

		//Fenneko brazo derecho
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.01f, 0.04f, 0.04f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FennekoText.UseTexture();
		der.RenderModel();


		///////////////////////////////////////////////////////////////////////////////////////// Terminan Personajes

		/********************************************************************************************************/
		//Lampara
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 20.0f));
		model = glm::translate(model, glm::vec3(3.8f, 0.7f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 20.0f));
		model = glm::translate(model, glm::vec3(10.5f, 0.7f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, movSilla_x * 5 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();
		/********************************************************************************************************/

		////////////////////////////////////////////////// Pecera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.2f, 0.5f, -66.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pecera.RenderModel();


		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetu += 0.001;
		toffsetv += 0.0;
		//para que no se desborde la variable
		if (toffsetu > 1.0)
			toffsetu = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetu, toffsetv);
		/*
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*No es animación si sólo tiene una transformación geométrica
		FlechaTexture.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();*/
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void musica2D() {
	//Inicia el módulo con los parámetros por defecto
	ISoundEngine* engine = createIrrKlangDevice();
	ISoundEngine* engine2 = createIrrKlangDevice();

	if (!engine) {
		printf("No se pudo iniciar\n");
	}

	engine->play2D("media/fondo.mp3", false);//Se reproduce audio de fondo, duración 1 minuto
	engine2->play2D("media/ambiental.mp3", true);//Se reproduce audio ambiental, continua en loop reprpduciendo

	//engine->drop(); 
}




void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movpapel_x es: %f\n", movpapel_x);
			//printf("movpapel_y es: %f\n", movpapel_y);
			printf(" \npresiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_K])
	{
		if (ciclo < 1)
		{
			//printf("movpapel_x es: %f\n", movpapel_x);
			movpapel_x += 1.0f;
			printf("\n movpapel_x es: %f\n", movpapel_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_J])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}





}
/*---------------------------------------------------------*/
/* ----------------   Práctica 11 --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------------- Alumno: Valderrama Navarro Armando ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include<time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1200;

// camera
Camera camera(glm::vec3(0.0f, 200.0f, 100.0f));
float MovementSpeed = 1.5f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 180.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		recorrido5 = false,
		recorrido6 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}


void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		if (recorrido1)
		{
			movAuto_z -= 1.5f;
			orienta = 180.0f;
			if (movAuto_z < -150.0f)
			{
				recorrido1 = false;
				recorrido4 = true;
			}
		}
		if (recorrido2)
		{
			movAuto_x -= 1.5f;
			orienta = -90.0f;
			if (movAuto_x < -200.0f)
			{
				recorrido2 = false;
				recorrido6 = true;
			}
		}
		if (recorrido3)
		{
			movAuto_z += 1.5f;
			orienta = 0.0f;
			if (movAuto_z > 150.0f)
			{
				recorrido3 = false;
				recorrido2 = true;
			}
		}
		if (recorrido4)
		{
			movAuto_x -= 1.5f;
			orienta = -90.0f;
			if (movAuto_x < -200.0f)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5)
		{
			movAuto_x += 2.0f;
			movAuto_z += 3.0f;
			orienta = 33.4248f;
			if (movAuto_z >= 150.0f)
			{
				recorrido5 = false;
				recorrido2 = true;
			}
		}
		if (recorrido6)
		{
			movAuto_x += 3.0f;
			movAuto_z -= 2.25f;
			orienta = 126.87f; // m = 0.75
			if (movAuto_x >= 0.0f)
			{
				recorrido6 = false;
				recorrido1 = true;
			}
		}
	}
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 11", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	//Shader staticShader("Shaders/lightVertex.vs", "Shaders/lightFragment.fs");
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	//Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model floor_house("resources/objects/PisoCasa/floor_house.obj");
	Model barda_exterior("resources/objects/BardaJardin/barda_jardin.obj");
	Model pasto("resources/objects/Pasto/pasto.obj");
	Model arbol_cafe("resources/objects/ArbolCafe/arbolcafe.obj");
	Model pared("resources/objects/Muro/muro.obj");
	Model librero("resources/objects/Librero/librero.obj");
	Model chimenea("resources/objects/Chimenea/chimenea1.obj");
	Model sofa("resources/objects/Sofa/sofa.obj");
	Model mesa("resources/objects/Mesa/mesa.obj");
	Model cama("resources/objects/Cama/cama.obj");
	Model cama2("resources/objects/Cama2/cama2.obj");
	Model mesam("resources/objects/MesaM/mesam.obj");
	Model sillam("resources/objects/SillaM/sillam.obj");
	Model mueblem("resources/objects/MuebleM/mueblem.obj");
	Model porteria("resources/objects/Porteria/porteriam.obj");
	Model mueble2("resources/objects/Porteria/mueble2.obj");
	Model roperom("resources/objects/RoperoM/roperom.obj");
	Model cuna("resources/objects/Cuna/cuna.obj");
	Model banco("resources/objects/Banco/banco.obj");
	Model balon("resources/objects/Balon/balon.obj");
	Model mueblej("resources/objects/MuebleJ/muebleJ.obj");


	//ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	//animacionPersonaje.initShaders(animShader.ID);

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.8f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(1.0f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Rememder to activate the shader with the animation
		/*animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-45.3f, -1.75f, 53.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		animacionPersonaje.Draw(animShader);]*/

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		//staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		
		/*---------------- EXTERIOR ----------------*/
		// PASTO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);


		// BARDA EXTERIOR
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);
	

		// ARBOLES
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, 0.0f, 700.0f));
		tmp = model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		arbol_cafe.Draw(staticShader);


		// ARBOLES
		model = glm::translate(tmp, glm::vec3(350.0f, 0.0f, -100.0f));
		staticShader.setMat4("model", model);
		arbol_cafe.Draw(staticShader);


		/*------------------ RECAMARA PADRES ----------------------*/
		// PARED CAMA PAPAS
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 25.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		// PARED FRONTAL CAMA PAPAS
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-35.0f, 25.0f, 162.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		// PARED VENTANA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-113.5f, 25.0f, -40.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.06f, 0.09f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		// PARED PATIO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(45.0f, 25.0f, -41.7f));
		model = glm::scale(model, glm::vec3(0.09f, 0.06f, 0.03f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		// PISO DE MADERA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 0.1f, 30.0f));
		model = glm::scale(model, glm::vec3(110.0f, 20.0f, 60.0f));
		staticShader.setMat4("model", model);
		floor_house.Draw(staticShader);
		/*------------------ FIN RECAMARA PADRES ----------------------*/



		/*----------------------- CLOSET ----------------------*/
		// PARED VENTANA
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-119.5f, 25.0f, -158.0f));
		model = glm::scale(model, glm::vec3(0.04f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		// PARED BAÑO
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-119.5f, 25.0f, -128.0f));
		model = glm::scale(model, glm::vec3(0.04f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);
		/*------------------ FIN CLOSET ----------------------*/


		/*----------------------- BAÑO ----------------------*/
		// PARED PASILLO
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-119.5f, 25.0f, -33.0f));
		model = glm::scale(model, glm::vec3(0.04f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);
		/*------------------ FIN BAÑO ----------------------*/


		/*----------------- CUARTO MALCOM --------------------*/
		// PARED FRONTAL CAMAS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, 25.0f, 232.5f));
		model = glm::scale(model, glm::vec3(0.053f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED VENTANA
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-190.0f, 25.0f, -115.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED CAMAS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, 25.0f, 391.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED PASILLO
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-221.0f, 25.0f, -33.5f));
		model = glm::scale(model, glm::vec3(0.072f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		// PISO DE MADERA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 0.1f, 256.5f));
		model = glm::scale(model, glm::vec3(110.0f, 20.0f, 45.0f));
		staticShader.setMat4("model", model);
		floor_house.Draw(staticShader);
		////////////////////////////////////////////////////////////////////////////////////////////////

		// CAMA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 0.1f, 323.0f));
		//model = glm::scale(model, glm::vec3(20.0f, 20.0f, 45.0f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);

		// CAMA2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 0.1f, 323.0f));
		//model = glm::scale(model, glm::vec3(20.0f, 20.0f, 45.0f));
		staticShader.setMat4("model", model);
		cama2.Draw(staticShader);

		// MESAM
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-92.0f, 0.1f, 201.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		mesam.Draw(staticShader);


		// SILLAM
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-92.0f, 0.1f, 218.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		sillam.Draw(staticShader);

		// MUEBLEM
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-35.0f, 0.1f, 260.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		mueblem.Draw(staticShader);


		// Mueble1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 0.1f, 295.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		porteria.Draw(staticShader);

		// Mueble2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 0.1f, 295.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		mueble2.Draw(staticShader);

		// ROPERO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-143.0f, 0.1f, 125.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		roperom.Draw(staticShader);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*----------------- FIN MALCOM --------------------*/


		/*----------------- PASILLO --------------------*/
		// PARED PATIO 1
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-46.5f, 25.0f, 1.5f));
		model = glm::scale(model, glm::vec3(0.05f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED PATIO 2
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-134.0f, 25.0f, 1.5f));
		model = glm::scale(model, glm::vec3(0.055f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);
		/*----------------- FIN PASILLO --------------------*/


		/*------------------ RECAMARA JAIME ----------------------*/
		// PARED JAMIE ESTE
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(95.4f, 25.0f, -42.3f));
		model = glm::scale(model, glm::vec3(0.09f, 0.06f, 0.05f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//PARED JAMIE NORTE
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(51.3f, 25.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.028f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//PARED JAMIE SUR

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(51.4f, 25.0f, 89.0f));
		model = glm::scale(model, glm::vec3(0.0281f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		// PARED JAMIE OESTE PEQUEÑA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 25.0f, 31.5f));
		model = glm::scale(model, glm::vec3(0.00001f, 0.06f, 0.008f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// CUNA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(45.0f, -1.0f, -28.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		cuna.Draw(staticShader);

		// BANCO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(14.0f, 0.0f, -24.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		banco.Draw(staticShader);

		// BALON
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, 13.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		balon.Draw(staticShader);

		// MuebleJ
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 23.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		mueblej.Draw(staticShader);



		/*----------------- CUARTO SALA --------------------*/
		// PARED TELE
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-350.0f, 25.0f, -115.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED VENTANA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0f, 25.0f, 551.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED COCINA/COMEDOR
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-381.0f, 25.0f, -16.0f));
		model = glm::scale(model, glm::vec3(0.072f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PISO DE MADERA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 0.1f, 450.5f));
		model = glm::scale(model, glm::vec3(110.0f, 20.0f, 45.0f));
		staticShader.setMat4("model", model);
		floor_house.Draw(staticShader);
		/*----------------- FIN SALA --------------------*/


		/*----------------- COCINA/COMEDOR--------------------*/
		// PARED ENTRADA PRINCIPAL
		model = glm::translate(glm::mat4(1.0f), glm::vec3(110.0f, 25.0f, 420.0f));
		model = glm::scale(model, glm::vec3(0.072f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED PATIO Y SALA/COMEDOR 1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(170.0f, 25.0f, 198.5f));
		model = glm::scale(model, glm::vec3(0.07f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED PATIO Y SALA/COMEDOR 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(272.0f, 25.0f, 198.5f));
		model = glm::scale(model, glm::vec3(0.058f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED SALA/COMEDOR
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-340.0f, 25.0f, 112.0f));
		model = glm::scale(model, glm::vec3(0.065f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Librero
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 368.5f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setMat4("model", model);
		librero.Draw(staticShader);

		//Chimenea
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-147.0f, 0.6f, 430.f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setMat4("model", model);
		chimenea.Draw(staticShader);

		//Sofá grande
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-70.0f, 0.6f, 410.f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		staticShader.setMat4("model", model);
		sofa.Draw(staticShader);

		//MesaNorte
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-90.0f, 0.6f, 450.f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		//MesaSur
		model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-90.0f, 0.6f, 398.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////7


		// PISO DE MADERA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(192.0f, 0.1f, 325.0f));
		model = glm::scale(model, glm::vec3(110.0f, 20.0f, 77.0f));
		staticShader.setMat4("model", model);
		floor_house.Draw(staticShader);
		/*----------------- FIN COCINA/COMEDOR --------------------*/




		/*----------------- SALA/COMEDOR --------------------*/
		// PARED ENTRADA PRINCIPAL
		model = glm::translate(glm::mat4(1.0f), glm::vec3(270.0f, 25.0f, 453.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.06f, 0.0f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED SALA/COMEDOR 1
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-312.5f, 25.0f, 315.5f));
		model = glm::scale(model, glm::vec3(0.08f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PARED SALA/COMEDOR 2
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-200.0f, 25.0f, 315.5f));
		model = glm::scale(model, glm::vec3(0.065f, 0.06f, 0.09f));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		/*----------------- FIN SALA/COMEDOR --------------------*/

		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
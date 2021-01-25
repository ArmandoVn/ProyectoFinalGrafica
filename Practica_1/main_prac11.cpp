/*---------------------------------------------------------*/
/* ---------------- Proyecto Final --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*-------------         Alumnos           ---------------*/
/*------------- Corona Vera Logan Alejandro---------------*/
/*------------- Kennedy Villa Carolina ---------------*/
///*------------- Valderrama Navarro Armando ---------------*/
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
Camera camera(glm::vec3(90.0f, 25.0f, 420.0f)); 
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
glm::vec3 lightPosition(20.0f, 30.0f, -200.0f);
glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);

//Sound
void sound();

// posiciones
float	movBanJx = 0.0f,
		movBanJz = 0.0f,
		movCama1 = 0.0f,
		movCama2 = 0.0f,
		movMueble1 = 0.0f,
		movMueble2 = 0.0f,
		movMueble3 = 0.0f,
		orienta = 180.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		recorrido5 = false,
		recorrido6 = false,
		recorridoBJ1 = false,
		recorridoBJ2 = false,
		animacionBJ = false,
		recorridoBJR1 = false,
		recorridoBJR2 = false,
		animacionBJR = false,
		animacionCM = false,
		recorridoM1 = false,
		recorridoM2 = false,
		recorridoC1 = false,
		recorridoC2 = false,
		recorridoM3 = false,
		animacionCMR = false,
		recorridoMR1 = false,
		recorridoMR2 = false,
		recorridoCR1 = false,
		recorridoCR2 = false,
		recorridoMR3 = false;

float initBottle = 270.0f,
	  initBottleY = 20.0f,
	  rotateBottle = 0.0f,
	  mariY = 13.0f,
	  mariZ = 25.0f;
//Keyframes (Manipulación y dibujo)
float	movAuto_x = 0.0f,
		movAuto_y = 0.0f,
		movAuto_z = 0.0f,
		giroAuto_y = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroAuto_yInc = 0.0f;

// Sound

bool activeAnim = false,
	 activeAnimM = false,
	 displayGlass = false,
			mari1 = true,
			mari2 = false;

bool soundon = true;


#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAuto_x;		//Variable para PosicionX
	float movAuto_y;		//Variable para PosicionY
	float movAuto_z;		//Variable para PosicionY
	float giroAuto_y;		//Variable para PosicionZ

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 5;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	std::cout << "X = " << movAuto_x << std::endl;
	std::cout << "Y = " << movAuto_y << std::endl;
	std::cout << "Z = " << movAuto_z << std::endl;
	std::cout << "GIRO Y = " << giroAuto_y << std::endl;

	KeyFrame[FrameIndex].movAuto_x = movAuto_x;
	KeyFrame[FrameIndex].movAuto_y = movAuto_y;
	KeyFrame[FrameIndex].movAuto_z = movAuto_z;
	KeyFrame[FrameIndex].giroAuto_y = giroAuto_y;

	FrameIndex++;
}

void resetElements(void)
{
	movAuto_x = KeyFrame[0].movAuto_x;
	movAuto_y = KeyFrame[0].movAuto_y;
	movAuto_z = KeyFrame[0].movAuto_z;
	giroAuto_y = KeyFrame[0].giroAuto_y;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].movAuto_x - KeyFrame[playIndex].movAuto_x) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].movAuto_y - KeyFrame[playIndex].movAuto_y) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].movAuto_z - KeyFrame[playIndex].movAuto_z) / i_max_steps;
	giroAuto_yInc = (KeyFrame[playIndex + 1].giroAuto_y - KeyFrame[playIndex].giroAuto_y) / i_max_steps;

}

// Sound
void sound() {
	if (soundon) {
		bool played = PlaySound("birds.wav", NULL, SND_LOOP | SND_ASYNC);
		cout << "Ambient:" << played << endl;
		soundon = false;
	}
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
			movAuto_x += incX;
			movAuto_y += incY;
			movAuto_z += incZ;
			giroAuto_y += giroAuto_yInc;

			i_curr_steps++;
		}
	}


	//Botella 
	if (activeAnim)
	{
		if (initBottle <= 275.0f)
		{
			initBottle += 0.1;
			if (rotateBottle <= 50.0f)
				rotateBottle += 1.0;
		}
		if (initBottleY >= -5.0f) {
			initBottleY -= 1.0f;
			if (initBottleY <= -6.0f)
				displayGlass = true;
		}
	}

	//////Mariposa
	if (activeAnimM) {
		
		if (mari1) {
			mariY += 1.0f;
			mariZ -= 0.8f;
			if (mariY >= 14.0f) {
				mari2 = true;
				mari1 = false;
			}
		}
		if (mari2) {
			mariY -= 1.0f;
			if (mariY <= -6.0f) {
				mari1 = true;
				mari2 = false;
			}
		}

	}


	//Animación Banco de Jaimie
	if (animacionBJ)//x = 14.0f, z=-24.5f
	{
		if (recorridoBJ1)
		{
			movBanJz += 1.0f;
			if (movBanJz > 20.0f)
			{
				recorridoBJ1 = false;
				recorridoBJ2 = true;
				
			}
		}

		if (recorridoBJ2)
		{
			movBanJx += 1.0f;
			if (movBanJx > 13.0f)
			{
				recorridoBJ2 = false;
			}

		}
	}
	//Animación Banco de Jaimie Regreso a la Cuna
	if (animacionBJR)//x = 14.0f, z=-24.5f
	{
		if (recorridoBJR1)
		{
			movBanJx -= 1.0f;
			if (movBanJx < 0.0f)
			{
				recorridoBJR1 = false;
				recorridoBJR2 = true;

			}
		}

		if (recorridoBJR2)
		{
			movBanJz -= 1.0f;
			if (movBanJz < 0.0f)
			{
				recorridoBJR2 = false;
			}

		}
	}

	//Animación de mover los muebles
	if (animacionCM)
	{
		if (recorridoM1)
		{
			movMueble1 -= 1.0f;
			if (movMueble1 < -40.0f)
			{
				recorridoM1 = false;
				recorridoC1 = true;

			}
		}

		if (recorridoC1)
		{
			movCama1 -= 1.0f;
			if (movCama1 < -37.0f)
			{
				recorridoC1 = false;
				recorridoM2 = true;
			}

		}

		if (recorridoM2)
		{
			movMueble2 -= 1.0f;
			if (movMueble2 < -40.0f)
			{
				recorridoM2 = false;
				recorridoC2 = true;

			}
		}

		if (recorridoC2)
		{
			movCama2 -= 1.0f;
			if (movCama2 < -37.0f)
			{
				recorridoC2 = false;
				recorridoM3 = true;
				
			}

		}
		if (recorridoM3)
		{
			movMueble3 += 1.0f;
			if (movMueble3 > 25.0f)
			{
				recorridoM3 = false;
				

			}

		}
	}


	//Animación de mover los muebles de regreso
	if (animacionCMR)
	{
		if (recorridoMR1)
		{
			movMueble1 += 1.0f;
			if (movMueble1 > 0.0f)
			{
				recorridoMR1 = false;
				recorridoCR2 = true;

			}
		}

		if (recorridoCR1)
		{
			movCama1 += 1.0f;
			if (movCama1 > 0.0f)
			{
				recorridoCR1 = false;
				recorridoMR1 = true;
			}

		}

		if (recorridoMR2)
		{
			movMueble2 += 1.0f;
			if (movMueble2 > 0.0f)
			{
				recorridoMR2 = false;
				recorridoMR3 = true;

			}
		}

		if (recorridoCR2)
		{
			movCama2 += 1.0f;
			if (movCama2 > 0.0f)
			{
				recorridoCR2 = false;
				recorridoMR2 = true;

			}

		}
		if (recorridoMR3)
		{
			movMueble3 -= 1.0f;
			if (movMueble3 < 0.0f)
			{
				recorridoMR3 = false;


			}

		}
	}

}



int main()
{
	// glfw: initialize and configure
	// 
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final", NULL, NULL);
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
	/*---------------- MODELOS CASA ----------------*/
	Model floor_house("resources/objects/PisoCasa/floor_house.obj");
	Model barda_exterior("resources/objects/BardaJardin/barda_jardin.obj");
	Model pasto("resources/objects/Pasto/pasto.obj");
	Model arbol_cafe("resources/objects/ArbolCafe/arbolcafe.obj");
	Model pared("resources/objects/Muro/muro.obj");
	Model puerta("resources/objects/Puerta/puerta.obj");
	Model librero("resources/objects/Librero/librero.obj");
	Model chimenea("resources/objects/Chimenea/chimenea1.obj");
	Model sofa("resources/objects/Sofa/sofa.obj");
	Model sofa1("resources/objects/Sofa1/sofa1.obj");
	Model mesa("resources/objects/Mesa/mesa.obj");
	Model cama("resources/objects/Cama/cama.obj");
	Model cama2("resources/objects/Cama2/cama2.obj");
	Model mesam("resources/objects/MesaM/mesam.obj");
	Model sillam("resources/objects/SillaM/sillam.obj");
	Model mueblem("resources/objects/MuebleM/mueblem.obj");
	Model porteria("resources/objects/Porteria/porteriam.obj");
	Model mueble2("resources/objects/Porteria/mueble2.obj");
	Model roperom("resources/objects/RoperoM/roperom.obj");
	
	Model banco("resources/objects/Banco/banco.obj");
	Model balon("resources/objects/Balon/balon.obj");
	Model mueblej("resources/objects/MuebleJ/muebleJ.obj");
	Model charger("resources/objects/Charger/charger.obj");
	Model pavimento("resources/objects/Pavimento/pavimento.obj");
	Model bodega("resources/objects/Bodega/bodega.obj");

	/*---------------- MODELOS COCINA ----------------*/
	Model cereal("resources/objects/ArticulosCocina/cajas.obj");
	Model alacena("resources/objects/Alacena/alacena.obj");
	Model refri("resources/objects/Refri/refri.obj");
	Model mesaCocina("resources/objects/Mesa/mesa00.obj");
	Model estufa("resources/objects/Estufa/Estufa_LSVC.obj");
	Model sink("resources/objects/Lavabo/Sink_LSVC.obj");
	Model silla("resources/objects/Silla/silla.obj");
	Model gabinete("resources/objects/Gabinetes/gabinete.obj");
	Model gabinete2("resources/objects/Gabinetes/gabinete2.obj");
	Model gabinete3("resources/objects/Gabinetes/gabinete3.obj");
	Model broken("resources/objects/Glass/broken.obj");
	Model bottle("resources/objects/Glass/bottle.obj");
	Model agua("resources/objects/Glass/water.obj");
	Model mari("resources/objects/Mariposa/mariposa.obj");
	/*---------------- MODELOS BAÑO ----------------*/
	Model ducha("resources/objects/Bathroom/ducha.obj");
	Model espejo("resources/objects/Bathroom/espejo.obj");
	Model lavabo("resources/objects/Bathroom/lavabo.obj");
	Model papel("resources/objects/Bathroom/papel.obj");
	Model paperholder("resources/objects/Bathroom/paperholder.obj");
	Model tina("resources/objects/Bathroom/tina.obj");
	Model toilet("resources/objects/Bathroom/toilet.obj");

	Model pool("resources/objects/Pool/pool.obj");

	Model salaG("resources/objects/Kennedy/salaGrande.obj");
	Model muebleTV("resources/objects/Kennedy/muebleTV.obj");
	Model mesaR("resources/objects/Kennedy/mesa.obj");
	Model sillaSala("resources/objects/Kennedy/sillaSala.obj");
	Model sillon("resources/objects/Kennedy/sillon.obj");
	Model libreroS("resources/objects/Kennedy/librero.obj");
	Model cuna("resources/objects/Kennedy/cuna.obj");
	Model cambiador("resources/objects/Kennedy/cambiadorP.obj");
	Model oso("resources/objects/Kennedy/oso.obj");
	Model ropero("resources/objects/Kennedy/ropero.obj");
	Model estufaJ("resources/objects/Kennedy/estufaJ.obj");

	//ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	//animacionPersonaje.initShaders(animShader.ID);

	//Inicialización de KeyFrames
	//for (int i = 0; i < MAX_FRAMES; i++)
	//{
	//	KeyFrame[i].movAuto_x = 0;
	//	KeyFrame[i].movAuto_y = 0;
	//	KeyFrame[i].movAuto_z = 0;
	//	KeyFrame[i].giroAuto_y = 0;
	//}

	KeyFrame[0].movAuto_x = -150;
	KeyFrame[0].movAuto_y = 0;
	KeyFrame[0].movAuto_z = 0;
	KeyFrame[0].giroAuto_y = 0;

	KeyFrame[1].movAuto_x = 310;
	KeyFrame[1].movAuto_y = 40;
	KeyFrame[1].movAuto_z = 0;
	KeyFrame[1].giroAuto_y = 0;

	KeyFrame[2].movAuto_x = 850;
	KeyFrame[2].movAuto_y = 0;
	KeyFrame[2].movAuto_z = 0;
	KeyFrame[2].giroAuto_y = 0;

	KeyFrame[3].movAuto_x = 850;
	KeyFrame[3].movAuto_y = 0;
	KeyFrame[3].movAuto_z = 0;
	KeyFrame[3].giroAuto_y = 90;

	KeyFrame[4].movAuto_x = 850;
	KeyFrame[4].movAuto_y = 0;
	KeyFrame[4].movAuto_z = -480;
	KeyFrame[4].giroAuto_y = 90;

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
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		//staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		
		/*---------------- EXTERIOR ----------------*/
		// PASTO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		// BARDA EXTERIOR
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		staticShader.setMat4("model", model);
		barda_exterior.Draw(staticShader);
	

		// ARBOLES
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-150.0f, 0.0f, 600.0f));
		tmp = model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		arbol_cafe.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(350.0f, 0.0f, -50.0f));
		staticShader.setMat4("model", model);
		arbol_cafe.Draw(staticShader);

		
		//PAVIMENTO
		model = glm::translate(glm::mat4(1.0f), glm::vec3(450.0f, 0.09f, 220.0f));
		tmp = model = glm::scale(model, glm::vec3(2.0f, 0.01f, 5.0f));
		staticShader.setMat4("model", model);
		pavimento.Draw(staticShader);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 38.0f));
		staticShader.setMat4("model", model);
		pavimento.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 38.0f));
		staticShader.setMat4("model", model);
		pavimento.Draw(staticShader);

		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-743.0f, 0.09f, 410.0f));
		tmp = model = glm::scale(model, glm::vec3(2.0f, 0.01f, 5.0f));
		staticShader.setMat4("model", model);
		pavimento.Draw(staticShader);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -38.0f));
		staticShader.setMat4("model", model);
		pavimento.Draw(staticShader);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -38.0f));
		staticShader.setMat4("model", model);
		pavimento.Draw(staticShader);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -38.0f));
		staticShader.setMat4("model", model);
		pavimento.Draw(staticShader);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -38.0f));
		staticShader.setMat4("model", model);
		pavimento.Draw(staticShader);

		tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -38.0f));
		staticShader.setMat4("model", model);
		pavimento.Draw(staticShader);


		// CHARGER
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f, 0.25f, 740.0f));
		model = glm::translate(model, glm::vec3(movAuto_x, movAuto_y, movAuto_z));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(giroAuto_y), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		charger.Draw(staticShader);


		// BODEGA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(450.0f, 0.0f, 37.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		bodega.Draw(staticShader);


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

		// PUERTA
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-116.5f, 0.0f, 20.5f));
		model = glm::scale(model, glm::vec3(1.2f, 1.7f, 2.4f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);

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

		// CAMA Cuarto papás
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-80.0f, 0.1f, 5.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		staticShader.setMat4("model", model);
		cama2.Draw(staticShader);

		// Mueble Cuarto papás
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-130.0f, 0.1f, -30.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		staticShader.setMat4("model", model);
		porteria.Draw(staticShader);

		// Mueble2 Cuarto papás
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-30.0f, 0.1f, -30.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		staticShader.setMat4("model", model);
		porteria.Draw(staticShader);
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

		// PUERTA BAÑO
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-116.5f, 0.0f, -45.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.7f, 2.4f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);

		/*---------------- MODELOS BAÑO ---------------------*/
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-35.0f, 2.0f, 182.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		staticShader.setMat4("model", model);
		toilet.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-50.0f, 20.0f, 188.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		staticShader.setMat4("model", model);
		paperholder.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-50.0f,19.5f, 187.5f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		staticShader.setMat4("model", model);
		papel.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-50.6f, 0.0f, 130.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		lavabo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-147.0f, 0.0f, 159.5f));
		model = glm::rotate(model, glm::radians(-270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		tina.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-114.0f, 30.0f, 165.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		ducha.Draw(staticShader);

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
	

		// PUERTA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-31.5f, 0.0f, 243.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.7f, 2.4f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);


		// CAMA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 0.1f, 323.0f + movCama2));
		//model = glm::scale(model, glm::vec3(20.0f, 20.0f, 45.0f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);

		// CAMA2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 0.1f, 323.0f + movCama1));
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
		model = glm::translate(model, glm::vec3(-35.0f, 0.1f, 260.0f + movMueble3));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		mueblem.Draw(staticShader);


		// Mueble1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-70.0f, 0.1f, 295.0f + movMueble1));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		porteria.Draw(staticShader);

		// Mueble2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 0.1f, 295.0f + movMueble2));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		mueble2.Draw(staticShader);

		// ROPERO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-143.0f, 0.1f, 125.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		roperom.Draw(staticShader);

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
		model = glm::translate(model, glm::vec3(2.0f, 25.0f, 33.0f));
		model = glm::scale(model, glm::vec3(0.00001f, 0.06f, 0.008f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		pared.Draw(staticShader);


		// PUERTA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 0.0f, 55.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.7f, 2.4f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);


		// CUNA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(45.0f, -1.0f, -28.5f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		cuna.Draw(staticShader);

		// Cambiador
		model = glm::translate(glm::mat4(1.0f), glm::vec3(8.0f, -1.0f, -26.5f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		cambiador.Draw(staticShader);
		// Oso
		model = glm::translate(model, glm::vec3(0.0f, 15.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setMat4("model", model);
		oso.Draw(staticShader);
		// BANCO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(14.0f + movBanJx, 0.0f, -24.5f + movBanJz));//x = 14.0f, z=-24.5f
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		banco.Draw(staticShader);

		// BALON
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(33.0f, 31.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		balon.Draw(staticShader);
		//Ropero
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(28.0f, 0.0f, 38.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		ropero.Draw(staticShader);
		//Estufa juguete
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(45.0f, 0.0f, 13.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		estufaJ.Draw(staticShader);


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

		//Sillon
		model = glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(400.0f, 10.0f, -245.f));
		model = glm::scale(model, glm::vec3(0.7f,0.8f,0.7f));
		staticShader.setMat4("model", model);
		sillon.Draw(staticShader);

		//Sofá grande Sala
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-420.0f, 10.0f, 210.f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		salaG.Draw(staticShader);
		
		//Mueble TV
		
		model = glm::translate(model, glm::vec3(-27.0f, 0.1f, -25.f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		muebleTV.Draw(staticShader);

		//Mesa Sala
		model = glm::translate(glm::mat4(1.0f), glm::vec3(190.0f, 18.0f, 260.f));
		model = glm::scale(model, glm::vec3(0.7f,0.72f,0.7f));
		staticShader.setMat4("model", model);
		mesaR.Draw(staticShader);

		//Silla Mesa Sala
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f,1.2f,1.0f));
		model = glm::translate(model, glm::vec3(185.0f, 8.0f, 240.f));
		staticShader.setMat4("model", model);
		sillaSala.Draw(staticShader);

		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::translate(model, glm::vec3(175.0f, 8.0f, 260.f));
		model = glm::rotate(model, glm::radians(75.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		sillaSala.Draw(staticShader);

		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::translate(model, glm::vec3(205.0f, 8.0f, 260.f));
		model = glm::rotate(model, glm::radians(-85.0f), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		sillaSala.Draw(staticShader);

		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-190.0f, 8.0f, -280.f));
		staticShader.setMat4("model", model);
		sillaSala.Draw(staticShader);

		//Librero
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-420.0f, 0.0f, 117.f));
		model = glm::scale(model, glm::vec3(0.72f));
		staticShader.setMat4("model", model);
		libreroS.Draw(staticShader);

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


		// PUERTA
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-418.0f, 0.0f, 80.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.7f, 2.4f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);

	
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



		// PISO DE MADERA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(192.0f, 0.1f, 325.0f));
		model = glm::scale(model, glm::vec3(110.0f, 20.0f, 77.0f));
		staticShader.setMat4("model", model);
		floor_house.Draw(staticShader);

		/*----------------- MODELOS COCINA --------------------*/
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, 0.2f, 214.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.18f, 0.25f, 0.18f));
		staticShader.setMat4("model", model);
		refri.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(205.5f, 37.0f, -80.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		alacena.Draw(staticShader);

		/*model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-250.0f, 42.0f, 70.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		cereal.Draw(staticShader);*/
		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-155.0f, 1.0f, 260.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(260.0f, 1.0f, 160.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(245.0f, 1.0f, 160.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-255.0f, 1.0f, -160.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-240.0f, 1.0f, -160.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(210.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(100.0f, 1.0f, -262.0f));
		model = glm::scale(model, glm::vec3(0.9f));
		staticShader.setMat4("model", model);
		silla.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-160.0f, 1.0f, 250.0f));
		model = glm::scale(model, glm::vec3(0.42f,0.52f, 0.42f));
		staticShader.setMat4("model", model);
		mesaCocina.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(86.0f, 1.0f, 145.0f));
		model = glm::scale(model, glm::vec3(4.0f,4.5f,4.0f));
		staticShader.setMat4("model", model);
		gabinete.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(72.0f, 1.0f, 213.0f));
		model = glm::scale(model, glm::vec3(5.0f, 4.5f, 5.0f));
		staticShader.setMat4("model", model);
		gabinete2.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(244.0f, 1.0f, -103.0f));
		model = glm::scale(model, glm::vec3(4.85f, 4.5f, 5.0f));
		staticShader.setMat4("model", model);
		gabinete3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(-113.0f, 16.0f, -245.0f));
		model = glm::scale(model, glm::vec3(0.45f, 1.5f, 0.7f));
		staticShader.setMat4("model", model);
		estufa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(201.0f, 12.6f, -55.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setMat4("model", model);
		sink.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(160.0f, mariY, mariZ)); //100,13,25 avanza con menos 
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		mari.Draw(staticShader);

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

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, 20.5f, 206.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.2f, 0.4f));
		staticShader.setMat4("model", model);
		agua.Draw(staticShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(110.0f, initBottleY, initBottle)); //110,20,270 posición inicial. Se mueve a la derecha
			model = glm::rotate(model, glm::radians(rotateBottle), glm::vec3(1.0f, 0.0f, 0.0)); //50.0f valor máximo para  propósitos de la animación 
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			staticShader.setMat4("model", model);
			bottle.Draw(staticShader);
		
		if (displayGlass) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(110.0f, 2.0f, 280.0f));
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			staticShader.setMat4("model", model);
			broken.Draw(staticShader);
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(250.0f, 1.0f, 55.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		staticShader.setMat4("model", model);
		pool.Draw(staticShader);
		
		glEnable(GL_BLEND);
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
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Sound
		sound();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly

void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//Camera movement 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model

	//Charger
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		movAuto_x +=10;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		movAuto_x -=10;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		movAuto_y += 10;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		movAuto_y -= 10;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		movAuto_z += 10;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		movAuto_z -= 10;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		giroAuto_y += 10;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		giroAuto_y -= 10;

	//Botella
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		activeAnim = true;
		initBottle++;
		initBottleY--;
		rotateBottle++;
	}
	//Mariposa
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		activeAnimM = true;
	}
		
	


	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;
	
	// Sound
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		soundon = false;

	//Banco de Jaimie guardado
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		animacionBJ ^= true;
		recorridoBJ1 = true;
	}

	//Banco de Jaimie de regreso a la cuna
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		animacionBJR ^= true;
		recorridoBJR1 = true;
	}

	//Mover muebles del cuarto de malcolm
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		animacionCM ^= true;
		recorridoM1 = true;
	}

	//Mover muebles del cuarto de malcolm
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		animacionCMR ^= true;
		recorridoCR1 = true;
	}

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
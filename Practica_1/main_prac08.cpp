/*---------------------------------------------------------*/
/* ----------------   Práctica 8 --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------------- Alumno: Valderrama Navarro Armando  ---------------*/
//#include "esfera.h"
// Biblioteca que permite leer diferentes formatos de archivos
#define STB_IMAGE_IMPLEMENTATION
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>


void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat lastX = SCR_WIDTH / 2.0f,
		lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;


void myData(void);
void display(Shader);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

//Texture
unsigned int	t_smile,
				t_toalla,
				t_unam,
				t_white,
				t_grafiti,
				t_papel,
				t_cube;

unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{
	// Pagina para obtener texturas:
	// https://www.textures.com/
	t_smile = generateTextures("Texturas/awesomeface.png", 1);
	t_toalla = generateTextures("Texturas/toalla.tga", 0);
	t_unam = generateTextures("Texturas/escudo_unam.png", 1);
	t_white = generateTextures("Texturas/white.jpg", 0);
	// IMPORTANTE: Las texturas siempre deben ser imagenes multiplos de dos y 
	// de perefencia de forma cuadrada.
	t_grafiti = generateTextures("Texturas/grafiti.png", 0); // 1 si es PNG, 0 otro JPG,
	t_papel = generateTextures("Texturas/papel.png", 1);
	t_cube = generateTextures("Texturas/cube.png", 1);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void myData()
{
	// IMPORTANTE: La defenicion de la textura siempre debe estar dada por
	// cordenadas entre 1 y 0, en caso de ser mayor la imagen se repetira
	// el numero de veces que hayamos definido.
	float vertices[] = {
		// positions          // texture coords
		// 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		// 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		//-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		//-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 

		-0.5f, -0.5f, 0.5f,		0.25f, 0.33f,	//Inferior izquierdo - Frontal
		0.5f, -0.5f, 0.5f,		0.5f, 0.33f,	//Inferior derecho
		0.5f, 0.5f, 0.5f,		0.5f, 0.66f,	//Superior derecho
		-0.5f, 0.5f, 0.5f,		0.25f, 0.66f,	//Superior izquierdo

		0.5f, -0.5f, -0.5f,		1.0f, 0.33f,	//I Derecho - Trasera
		-0.5f, -0.5f, -0.5f,	0.75f, 0.33f,	//I Izquierdo
		-0.5f, 0.5f, -0.5f,		0.75f, 0.66f,	//S Izquierdo
		0.5f, 0.5f, -0.5f,		1.0f, 0.66f,	//S Derecho

		-0.5f, 0.5f, 0.5f,		0.0f, 0.66f,	//S Izquierdo - Izq
		-0.5f, 0.5f, -0.5f,		0.25f, 0.66f,	//S Derecho
		-0.5f, -0.5f, -0.5f,	0.25f, 0.33f, //I Derecho
		-0.5f, -0.5f, 0.5f,		0.0f, 0.33f,	//I Izquierdo

		0.5f, 0.5f, 0.5f,		0.75f, 0.66f,	//S Derecho - Der
		0.5f, -0.5f, 0.5f,		0.75f, 0.33f,	//I Derecho
		0.5f, -0.5f, -0.5f,		0.5f, 0.33f,	//I Izquierdo
		0.5f, 0.5f, -0.5f,		0.5f, 0.66f,	//S Izquierdo

		-0.5f, 0.5f, 0.5f,		0.25f, 1.0f,	//S Izquierdo - Sup
		0.5f, 0.5f, 0.5f,		0.5f, 1.0f,	//S Derecho
		0.5f, 0.5f, -0.5f,		0.5f, 0.66f,	//I Derecho
		-0.5f, 0.5f, -0.5f,		0.25f, 0.66f,	//I Izquierdo

		-0.5f, -0.5f, 0.5f,		0.25f, 0.0f,	//I Izquierdo - Inf
		-0.5f, -0.5f, -0.5f,	0.25f, 0.33f,	//S Izquierdo
		0.5f, -0.5f, -0.5f,		0.5f, 0.33f,	//S Derecho
		0.5f, -0.5f, 0.5f,		0.5f, 0.0f,	//I Derecho
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void animate(void)
{

}

void display(Shader shader)
{
	//Enable Shader
	shader.use();

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setVec3("viewPos", camera.Position);
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí

	// Elemento A
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_cube); // Recibe la textura que deseo utilizar
	glDrawArrays(GL_QUADS, 0, 24); //A

	//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	//model = glm::scale(model, glm::vec3(4.0f, 1.5f, 1.0f));
	//shader.setMat4("model", model);
	//shader.setVec3("aColor", 1.0f, 1.0f, 1.0f); // Se puede sumar el color del objeto a la textura
	//glBindTexture(GL_TEXTURE_2D, t_grafiti); // Recibe la textura que deseo utilizar
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	//// El blending o efecto de agua/cristal siempre debe ser declarado al final de nuestros elementos para que funcione de manera correcta.
	//glEnable(GL_BLEND); // Activamos el Blend que es el metodo encargado de realizar el efecto de agua o cristal.
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Indicamos el algoritmo.
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//shader.setMat4("model", model);
	//shader.setVec3("aColor", 1.0f, 1.0f, 1.0f); // Se puede sumar el color del objeto a la textura
	//// Este comando se queda activo hasta que encuentre un nuevo GL_TEXTURE_2D
	//glBindTexture(GL_TEXTURE_2D, t_unam); // Recibe la textura que deseo utilizar
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDisable(GL_BLEND);


	glBindVertexArray(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 8", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();

	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	//my_sphere.init();
	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		//double currentFrame = glfwGetTime(); 
		lastFrame = SDL_GetTicks();
		
		//std::cout <<"frame time = " << deltaTime << "sec"<< std::endl;
        // input
        // -----
		// render
		// Backgound color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		my_input(window);
		animate();
		//Mi función de dibujo
		display(projectionShader);
		//lastFrame = currentFrame;
		deltaTime = SDL_GetTicks() - lastFrame;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
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
	

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
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

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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
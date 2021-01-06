/*---------------------------------------------------------*/
/* ----------------   Práctica 7 --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------------- Alumno: Valderrama Navarro Armando ---------------*/
#include "esfera.h"
#include "camera.h"

Esfera my_sphere(1.0f);

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, lightVAO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
/*double	lastX = 0.0f,
		lastY = 0.0f;*/
GLfloat lastX = SCR_WIDTH / 2.0f,
		lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

void myData(void);
void display(Shader, Shader);
void getResolution(void);
void animate(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

float	sol = 0.0f,
		year = 0.0f,
		day = 0.0f,
		moon = 0.0f,
		mars_year = 0.0f,
		jupiter_year = 0.0f,
		mercurio = 0.0f;


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
		GLfloat vertices[] = {
		//Position				//Normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//Trasera
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,//Frontal
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,//Izquierda
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,//Derecha
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,//Inferior
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,//Superior
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//To configure Second Object to represent Light
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void animate(void)
{
	sol += 1.0f;
	mercurio += 0.9f;
	year += 1.0f;
}

void display(Shader shaderProjection, Shader shaderLamp)
{
	
	//To Use Lighting
	shaderProjection.use();
	shaderProjection.setVec3("lightColor", 1.0f, 1.0f, 1.0f); // Color de la luz
	shaderProjection.setVec3("lightPos", lightPos); // Posición de la fuente de luz.

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 tmp1 = glm::mat4(1.0f);

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shaderProjection.setVec3("viewPos", camera.Position);
	shaderProjection.setMat4("model", model);
	shaderProjection.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shaderProjection.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí

	//Colocar código aquí
	shaderLamp.use();
	// Shaderlamp necesita las matrices de projecccion y vista
	shaderLamp.setMat4("projection", projection);
	shaderLamp.setMat4("view", view);
	model = glm::rotate(model, glm::radians(sol), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	shaderLamp.setMat4("model", model);
	// shaderProjection.setVec3("ambientColor", 1.0f, 1.0f, 0.0f);
	// shaderProjection.setVec3("diffuseColor", 1.0f, 1.0f, 0.0f);
	// shaderProjection.setVec3("specularColor", 1.0f, 1.0f, 0.0f);
	my_sphere.render();	//Sun

	shaderProjection.use();

	model = glm::rotate(glm::mat4(1.0f), glm::radians(mercurio), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
	shaderProjection.setMat4("model", model);
	// Componente que afecta a las caras menos iluminadas
	shaderProjection.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	// Componente que afecta a las caras más iluminadas del objeto
	shaderProjection.setVec3("diffuseColor", 0.9f, 0.9f, 0.9f);
	// Componente asociada al brillo de la figura
	shaderProjection.setVec3("specularColor", 1.0f, 1.0f, 1.0f);
	my_sphere.render();	//Mercurio

	model = glm::rotate(glm::mat4(1.0f), glm::radians(year), glm::vec3(0.0f, 1.0f, 0.0f));
	tmp1 = model = glm::translate(model, glm::vec3(14.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("diffuseColor", 0.0f, 0.0f, 1.0f);
	my_sphere.render();	//Tierra

	model = glm::rotate(tmp1, glm::radians(year), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 3.6f, 0.0f));
	model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("diffuseColor", 0.9f, 0.9f, 0.9f);
	my_sphere.render();	//Luna

	model = glm::rotate(glm::mat4(1.0f), glm::radians(year), glm::vec3(0.0f, 0.0f, 1.0f));
	tmp1 = model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mercurio), glm::vec3(1.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("diffuseColor", 1.0f, 0.0f, 0.0f);
	my_sphere.render();	//Marte

	model = glm::rotate(glm::mat4(1.0f), glm::radians(165.5f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(mercurio), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(26.9f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("diffuseColor", 01.0f, 0.0f, 1.0f);
	my_sphere.render();	//Jupiter

	model = glm::rotate(glm::mat4(1.0f), glm::radians(145.3f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(mercurio), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 36.9f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("diffuseColor", 0.0f, 1.0f, 1.0f);
	my_sphere.render();	//New planet

	/* 
	model = glm::rotate(model, glm::radians(sol), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f, 0.7f, 0.5f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("ambientColor", 0.6f, 0.6f, 0.6f);
	shaderProjection.setVec3("diffuseColor", 1.0f, 0.0f, 0.0f);
	shaderProjection.setVec3("specularColor", 0.0f, 0.0f, 0.0f);
	//my_sphere.render();	//Rectangle
	glDrawArrays(GL_TRIANGLES, 0, 36);	//Light

	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
	shaderProjection.setMat4("model", model);
	shaderProjection.setVec3("ambientColor", 1.0f, 1.0f, 1.0f);
	shaderProjection.setVec3("diffuseColor", 0.0f, 0.0f, 1.0f);
	shaderProjection.setVec3("specularColor", 0.8f, 0.8f, 0.8f);
	my_sphere.render();	//Sphere
	//glDrawArrays(GL_TRIANGLES, 0, 36);	//Light
	*/

	// Declaracion de la lampara
	// Este shader lo activamos porque sabemos que los vectores
	// de luz salen de la figura por consecuencia se haran mal los
	// calculos y entonces pintamos directamente de color amarillo la lampara
	/*shaderLamp.use();
	shaderLamp.setMat4("projection", projection);
	shaderLamp.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(glm::mat4(1.0f), lightPos);
	model = glm::scale(model, glm::vec3(0.25f));
	shaderLamp.setMat4("model", model);
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);	//Light
	*/

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 7", NULL, NULL);
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
	myData();
	my_sphere.init();
	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");
	//Shader projectionShader("shaders/shader_light_Gouraud.vs", "shaders/shader_light_Gouraud.fs");
	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");


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
		display(projectionShader, lampShader);
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
	glDeleteVertexArrays(1, &lightVAO);
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
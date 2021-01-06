/*---------------------------------------------------------*/
/* ----------------   Práctica 4 --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------------- Alumno: Valderrama Navarro Armando ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(Shader);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f,
		rotY = 0.0f,
		rotZ = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(Shader shader)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	

	shader.use();

	// create transformations and Projection
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 tmp, tmp2, tmp3 = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//projection = glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, 0.1f, 10.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ)); // Translación asiciada a la amtriz de vista.
	// Para realizar las rotaciones debemos usar un eje de referencia, de preferencia debe ser el del plano (x, y, z).
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	glBindVertexArray(VAO);
	//Dibujo del personaje

	// Aplicamos una tranformacion de escala
	// El primer argumento es el factor de escala en X, el segundo en Y y el tercero en Z. 
	
	model = glm::scale(model, glm::vec3(4.0f, 5.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Pecho

	tmp = model = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 2.75f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); // cuello

	model = glm::translate(tmp, glm::vec3(0.0f, 1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); // cabeza

	tmp = glm::mat4(1.0f);

	tmp = model = glm::translate(tmp, glm::vec3(-2.75f, 2.25f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Hombro izquierdo

	tmp = model = glm::translate(tmp, glm::vec3(-0.25f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 3.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Brazo izquierdo

	tmp = glm::mat4(1.0f);

	tmp = model = glm::translate(tmp, glm::vec3(2.75f, 2.25f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 0.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Hombro derecho

	model = glm::translate(tmp, glm::vec3(0.25f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 3.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Brazo derecho

	tmp = model = glm::translate(tmp, glm::vec3(0.25f, -3.9f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 0.3f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Mango espada

	model = glm::translate(tmp, glm::vec3(1.15f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.6f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Division espada

	model = glm::translate(tmp, glm::vec3(3.05f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.5f, 0.3f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Filo espada

	tmp = glm::mat4(1.0f);

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, -3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Coxis

	model = glm::translate(tmp, glm::vec3(-1.25f, -2.25f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 3.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.75f, 2.5f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Pierna izquierda

	model = glm::translate(tmp, glm::vec3(1.25f, -2.25f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 3.5f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.75f, 2.5f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Pierna derecha

	model = glm::translate(tmp, glm::vec3(-1.75f, -4.375f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 0.75f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Pie izquierda

	model = glm::translate(tmp, glm::vec3(1.75f, -4.375f, 0.0f));
	model = glm::scale(model, glm::vec3(2.5f, 0.75f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24); // Pie derecho

	/*----------------------- POLLO --------------------*/

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 8.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24);	//Cuerpo

	tmp = model = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, -2.5f, 1.5f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 7.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glDrawArrays(GL_QUADS, 0, 24);	//Pecho

	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.5f));
	model = glm::scale(model, glm::vec3(2.5f, 3.9f, 7.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.91f, 0.91f, 0.91f));
	glDrawArrays(GL_QUADS, 0, 24);	//Cola

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(tmp, glm::vec3(6, 2, 3));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.97f, 0.96f, 0.39f));
	glDrawArrays(GL_QUADS, 0, 24);	//Alas

	tmp = model = glm::translate(glm::mat4(1.0f), glm::vec3(-14.0f, 3.0f, -1.0f));
	model = glm::scale(model, glm::vec3(4.2f, 0.5f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);	//Ojos

	model = glm::translate(model, glm::vec3(0.0f, 3.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1 / 4.2f, 1 / 0.5f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);	//Cresta

	tmp = model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, -0.5f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.35f, 0.13f));
	glDrawArrays(GL_QUADS, 0, 24);	//Pico

	model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 3.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_QUADS, 0, 24);	//caruncula

	tmp3 = model = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, -5.5f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 2.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.35f, 0.13f));
	glDrawArrays(GL_QUADS, 0, 24);	//Pata derecha

	tmp2 = model = glm::translate(glm::mat4(1.0f), glm::vec3(-13.0f, -5.5f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 2.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.35f, 0.13f));
	glDrawArrays(GL_QUADS, 0, 24);	//Pata izquierda

	tmp2 = model = glm::translate(tmp2, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 0.25f, 1.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.35f, 0.13f));
	glDrawArrays(GL_QUADS, 0, 24);	//Pie izquierdo

	tmp2 = model = glm::translate(tmp2, glm::vec3(-0.5f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.25f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.35f, 0.13f));
	glDrawArrays(GL_QUADS, 0, 24);	//Garra izquierda 1

	model = glm::translate(tmp2, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.25f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.35f, 0.13f));
	glDrawArrays(GL_QUADS, 0, 24);	//Garra izqueirda 2

	tmp3 = model = glm::translate(tmp3, glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 0.25f, 1.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.35f, 0.13f));
	glDrawArrays(GL_QUADS, 0, 24);	//Pata derecha

	tmp3 = model = glm::translate(tmp3, glm::vec3(-0.5f, 0.0f, -1.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.25f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.35f, 0.13f));
	glDrawArrays(GL_QUADS, 0, 24);	//Garra derecha 1

	model = glm::translate(tmp3, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.25f, 2.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 0.35f, 0.13f));
	glDrawArrays(GL_QUADS, 0, 24);	//Garra derecha 2
	

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display(projectionShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.03f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.03f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		movY += 0.03f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		movY -= 0.03f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.03f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.03f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		rotX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		rotX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		rotZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		rotZ += 0.08f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}
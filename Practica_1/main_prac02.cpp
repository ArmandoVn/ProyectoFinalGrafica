/*---------------------------------------------------------*/
/* ----------------   Práctica 2 --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------------- Valderramana Navarro Armando ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint shaderProgramRed, shaderProgramColor;

static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec4 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);							\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderRed = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 0.0f, 0.0f);									\n\
}";

static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	

	float vertices[] = 
	{
		// Vectors for letter C
		-0.8f,  0.9f, 0.0f, 1.0f, 1.0f, 1.0f, // top right -> V0
		-0.4f,  0.9f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right -> V1
		-0.4f,  0.7f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom left -> V2
		-0.65f,  0.7f, 0.0f, 0.0f, 1.0f, 0.0f,  // top left -> V3
		-0.65f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // middle left -> V4
		-0.4f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // middle left -> V4
		-0.4f,  0.3f, 0.0f, 1.0f, 0.0f, 1.0f,  // middle left -> V4
		-0.8f,  0.3f, 0.0f, 0.0f, 0.8f, 1.0f,  // middle left -> V4

		// Vectors for letter A
		-0.3f,  0.3f, 0.0f, 1.0f, 1.0f, 1.0f, // top right -> V0
		-0.1f,  0.3f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right -> V1
		0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom left -> V2
		0.1f,  0.3f, 0.0f, 0.0f, 1.0f, 0.0f,  // top left -> V3
		0.3f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f,  // middle left -> V4
		0.05f,  0.9f, 0.0f, 1.0f, 1.0f, 1.0f, // middle left -> V4
		-0.05f,  0.9f, 0.0f, 0.5f, 0.8f, 1.0f,  // middle left -> V4
		0.0f,  0.7f, 0.0f, 0.3f, 0.0f, 1.0f,  // middle left -> V4
		-0.025f,  0.6f, 0.0f, 0.0f, 1.0f, 1.0f, // middle left -> V4
		0.025f,  0.6f, 0.0f, 0.2f, 0.0f, 1.0f,  // middle left -> V4

		// Vectors for letter V
		0.4f,  0.3f, 0.0f, 1.0f, 1.0f, 1.0f, // top right -> V0
		0.6f,  0.3f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right -> V1
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom left -> V2
		0.15f,  0.9f, 0.0f, 0.0f, 1.0f, 0.0f,  // top left -> V3
		0.35f,  0.9f, 0.0f, 0.0f, 0.0f, 1.0f,  // middle left -> V4
		0.65f,  0.9f, 0.0f, 0.0f, 0.0f, 1.0f, // middle left -> V4
		0.85f,  0.9f, 0.0f, 0.0f, 0.0f, 1.0f,  // middle left -> V4

		// Vectors for star
		-0.1f,  -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, // top right -> V0
		0.0f,  -0.1f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right -> V1
		0.1f,  -0.2f, 0.0f, 1.0f, 0.0f, 1.0f, // bottom left -> V2
		-0.065f,  -0.05f, 0.0f, 0.0f, 1.0f, 0.0f,  // top left -> V3
		0.0f,  0.0f, 0.0f, 0.9f, 0.0f, 1.0f,  // middle left -> V4
		0.065f,  -0.05f, 0.0f, 0.0f, 0.0f, 1.0f, // middle left -> V4
		-0.15f,  0.05f, 0.0f, 1.0f, 0.0f, 1.0f,  // middle left -> V4
		-0.04f,  0.05f, 0.0f, 0.0f, 0.0f, 1.0f,  // middle left -> V4
		0.0f,  0.2f, 0.0f, 1.0f, 0.0f, 1.0f, // middle left -> V4
		0.04f,  0.05f, 0.0f, 0.8f, 0.0f, 1.0f,  // middle left -> V4
		0.15f,  0.05f, 0.0f, 0.0f, 0.2f, 1.0f,  // middle left -> V4
	};

	unsigned int indices[] =
	{
		// C indices
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 7,
		7, 4, 6,
		4, 5, 6,

		// A indices
		8, 9, 10,
		8, 10, 16,
		8, 15, 16,
		8, 14,16,
		16, 15, 14,
		16, 10, 17,
		17, 10, 11,
		11,12,17,
		12,17,13,
		13,15,17,
		13,14,15,

		// V indices
		18, 19, 20,
		18, 20, 21,
		21, 22, 20,
		19, 20, 24,
		20, 24, 23,

		// Star indices
		25,26,29,
		25,28,29,
		31,28,29,
		31,32,29,
		32,33,29,
		33,34,29,
		34,35,29,
		35,30,29,
		30,27,29,
		27,26,29,
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRed, 1, &myFragmentShaderRed, NULL);
	glCompileShader(fragmentShaderRed);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometría con Color
	shaderProgramRed = glCreateProgram();
	glAttachShader(shaderProgramRed, vertexShader);
	glAttachShader(shaderProgramRed, fragmentShaderRed);
	glLinkProgram(shaderProgramRed);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderRed);
	glDeleteShader(fragmentShaderColor);

}

void display(void)
{
	glUseProgram(shaderProgramColor);

	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glPointSize(5.0); 
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 51, GL_UNSIGNED_INT, (void*)(18 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 66, GL_UNSIGNED_INT, (void*)(51 * sizeof(float)));
	glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_INT, (void*)(66 * sizeof(float)));
	//glDrawArrays(GL_LINE_LOOP, 0, 8);


	glBindVertexArray(0);

	glUseProgram(0);

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 2", NULL, NULL);
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
	//Configurar Shaders
	setupShaders();
    

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		//Mi función de dibujo
		display();

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}
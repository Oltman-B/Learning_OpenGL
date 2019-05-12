#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


// Source code for vertex shader test. glPosition is defined by OpenGL behind the scenes
const char *vertexShaderSrce =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n"
"vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";

// Source code for fragment shader in GLSL
const char *fragShaderSrce =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"FragColor = vertexColor;\n"
"}\n";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGlTest", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//********************OPENGL INITIALIZATION CODE************************

	// *********************START SHADER SETUP CODE***************************
	// Create shader object and compile vertex shader // Create a vertex shader and store the id
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderID, 1, &vertexShaderSrce, NULL);
	glCompileShader(vertexShaderID);

	// Check that shader compiled
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create fragment shader object and compile
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragShaderSrce, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create and link the shader program
	unsigned int shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID); // Link the shaders together, this is where input and output errors will be found

	// No longer need shader objects once linked into the program.
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	//*******************END SHADER SETUP CODE******************************

	float vertices1[] = {
	 -1.0f,  1.0f, 0.0f,
	 -0.5f,  1.0f, 0.0f,
	 -0.5f,  0.0f, 0.0f,
	 -1.0f,  0.0f, 0.0f
	};

	unsigned int indices1[] = {
		0, 1, 3,
		3, 2, 1
	};

	// Create element buffer object for the index buffer
	unsigned int EBO[2], VBO[2], VAO[2];

	// Create a Vertex Array Object to store the state of VertexArrayAttributes and the VBO associated with the array attributes
	// Makes drawing new objects easy because the attributes need to be set up only once for each vertex configuration.
	glGenVertexArrays(2, VAO);

	// Instantiate a buffer and store its iD in VBO
	glGenBuffers(2, VBO);
	// Create element buffer object
	glGenBuffers(2, EBO);

	// Bind the vertex array object first, then bind and setup the vertex buffers and vertex attributes
	glBindVertexArray(VAO[0]);

	// Bind the VBO buffer to a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	// Copy the vertex array into the VBObuffer with a static draw memory access hint to graphics card.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	// Set up the index array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	// Tell OpenGL how to interpret the vertex data (where & how to get the buffer's vertex data for the shader attribute.)
	//First parameter is the layout (location = 0) of position attribute set in shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	// Layout location 0 set in shader 
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0); Don't need to unbind because it is rebound with a different VAO in a few lines.

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//*******************END OPENGL INITIALIZATION CODE*************************

	// 'Game loop'
	while (!glfwWindowShouldClose(window))
	{
		// Input
		ProcessInput(window);

		//*********************Start Rendering Code***********************

		// Clear buffer to some color between renders
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		// Set which buffer to clear
		glClear(GL_COLOR_BUFFER_BIT);

		// Set shaderProgram as active shaderprogram
		glUseProgram(shaderProgramID);

		// Bind the vertext array object, only need to bind each iteration if it changes.
		glBindVertexArray(VAO[0]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, 6)

		// glBindVertexArray(0); unbind the VAO if you don't want the VBO or Vertex array attributes to be altered accidently.

		//**************End Rendering Code********************************

		// Check events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, VAO);
	glDeleteVertexArrays(1, VBO);
	glDeleteVertexArrays(1, EBO);

	glfwTerminate();
	return 0;
}
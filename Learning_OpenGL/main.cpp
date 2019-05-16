#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include <cmath>
#include "Shader.h"


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

	//***********************LOAD TEXTURE IMAGE*******************************
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;

	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// *********************START SHADER SETUP CODE***************************
	Shader ourShader("shader.vert", "shader.frag");
	//*******************END SHADER SETUP CODE******************************

	float vertices1[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices1[] = {
		0, 1, 2, 2, 3, 0
	};

	// Create element buffer object for the index buffer
	unsigned int EBO[1], VBO[1], VAO[1];

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//texture coordinate
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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

		ourShader.use();

		//Bind texture
		glBindTexture(GL_TEXTURE_2D, texture);
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
// Source code for vertex shader test. glPosition is defined by OpenGL behind the scenes
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f); // multiply transformation matrix by the position vector for each vertex
    ourColor = aColor;
	TexCoord = aTexCoord;
}
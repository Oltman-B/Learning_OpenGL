#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float alphaBlend;

void main()
{   
	float blendFactor;
	if(alphaBlend > 1)
	{
		blendFactor = 1;
	}
	else if(alphaBlend < 0)
	{
		blendFactor = 0;
	}
	else
	{
		blendFactor = alphaBlend;
	}
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), blendFactor);
}
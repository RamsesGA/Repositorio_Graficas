////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;

in vec2 TexCoord;
in vec4 colorfiesta;
//out vec3 color;
//out vec3 color;
uniform sampler2D renderedTexture;

layout(location = 0) out vec3 color;
void main() 
{
	color = texture(renderedTexture, TexCoord).rgb;
	color = color * colorfiesta.rgb;
	//color = vec3( 1.0,0.5,0.3 );
}
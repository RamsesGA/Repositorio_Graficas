////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;

in vec2 TexCoord;

uniform sampler2D textureDifuse;

layout(location = 0) out vec3 color;
void main() 
{
	// Inverted the TeexCoords y
	vec2 texC = vec2(TexCoord.x, -TexCoord.y);
	
	color = texture(textureDifuse, texC).rgb;
}
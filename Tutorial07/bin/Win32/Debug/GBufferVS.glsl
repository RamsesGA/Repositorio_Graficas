////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.vs
////////////////////////////////////////////////////////////////////////////////
#version 400


layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 TexCoord;
out vec4 colorfiesta;
//out vec4 wPosition;
//out mat4 wPosition;

uniform mat4 WM;
uniform mat4 VM;
uniform mat4 PM;
uniform mat4 MM;
uniform vec4 fiesta;

void main()
{
	mat4 VP = PM * VM;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = VP * WM * vec4(inVertexPosition, 1.0f);

	// Calculate position in world space
	//wPosition = WM * vec4(inVertexPosition, 1.0f);

	// Store the texture coordinates for the pixel shader.
	TexCoord = inTexCoord;
	colorfiesta = fiesta;
	//printf(TexCoord);
}
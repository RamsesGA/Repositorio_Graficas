////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.vs
////////////////////////////////////////////////////////////////////////////////
#version 400


layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 TexCoord;
uniform mat4 WordMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	// Calculate the View Projection Matrix
	mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = ViewProjectionMatrix * WordMatrix * vec4(inVertexPosition, 1.0f);


	// Store the texture coordinates for the pixel shader.
	TexCoord = inTexCoord;
}
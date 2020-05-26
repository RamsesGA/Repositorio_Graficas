////////////////////////////////////////////////////////////////////////////////
// Filename: OpenGLBufferVS.vs
////////////////////////////////////////////////////////////////////////////////
#version 400

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec3 Norm;
layout(location = 2) in vec2 inTexCoord;

out vec3 outNormal;
out vec2 TexCoord;
uniform mat4 WordMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 LightDir;

void main()
{
	// Calculate the View Projection Matrix
	mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = ViewProjectionMatrix * WordMatrix * vec4(inVertexPosition, 1.0f);

	outNormal = Norm * WordMatrix;

	// Store the texture coordinates for the pixel shader.
	TexCoord = inTexCoord;

}
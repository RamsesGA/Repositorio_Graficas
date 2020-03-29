////////////////////////////////////////////////////////////////////////////////
// Filename: Shadow.vs
////////////////////////////////////////////////////////////////////////////////
#version 400


/////////////////////
// INPUT VARIABLES //
/////////////////////
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inBinormal;
layout(location = 4) in vec3 inTangent;


//////////////////////
// OUTPUT VARIABLES //
//////////////////////


///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 WM;// world matrix
uniform mat4 SVM;// Shadow view matrix
uniform mat4 SPM;// shadow proj matrix
uniform mat4 RM;// Refl matrix

mat4 biasMatrix = mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main()
{
	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = SPM * SVM * RM * WM * vec4(inPosition, 1.0f);
}


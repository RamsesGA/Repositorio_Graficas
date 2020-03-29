////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.vs
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
out vec2 TexCoord;
out vec4 wPosition;
out mat3 TangentToWorld;
out vec4 ShadowCoord;
out vec2 TexCoordR;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 WM;
uniform mat4 VM;
uniform mat4 PM;
uniform mat4 RM;
uniform mat4 SVM;
uniform mat4 SPM;


mat4 biasMatrix = mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Calculate the TBN Matrix
	TangentToWorld[0] = (WM * vec4(inTangent, 0.0f)).xyz;
	TangentToWorld[1] = (WM * vec4(inBinormal, 0.0f)).xyz;
	TangentToWorld[2] = (WM * vec4(inNormal, 0.0f)).xyz;

	mat4 VP = PM * VM * RM;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_Position = VP * WM * vec4(inPosition, 1.0f);
		
	// Calculate position in world space
	wPosition = WM * vec4(inPosition, 1.0f);

	// Store the texture coordinates for the pixel shader.
	TexCoord = inTexCoord;
	
	// calc position in camera space
	ShadowCoord = biasMatrix * SPM * SVM * WM * vec4(inPosition, 1.0f);
	
	vec4 vClipReflection = PM * WM * vec4(inPosition.xy, 0.0, 1.0);
	vec2 vDeviceReflection = vClipReflection.st / vClipReflection.q;
	TexCoordR = vec2(0.5, 0.5) + 0.5 * vDeviceReflection;
}
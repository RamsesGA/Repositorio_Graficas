////////////////////////////////////////////////////////////////////////////////
// Filename: GBuffer.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;

/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec2 TexCoord;
in vec2 TexCoordR;
in vec4 wPosition;
in mat3 TangentToWorld;
in vec4 ShadowCoord;


//////////////////////
// OUTPUT VARIABLES //
//////////////////////
layout(location = 0) out vec4 Albedo;
layout(location = 1) out vec4 NormalRoughness;
layout(location = 2) out vec4 PositionMetallic;
layout(location = 3) out vec4 ShadowPos;


///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform sampler2D A;// A
uniform sampler2D M;// Metallic
uniform sampler2D N;// Normal
uniform sampler2D R;// Roughness
uniform sampler2D RT;// Reflected texture


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Take textures samplers
	vec3 AColor = texture(A, TexCoord).xyz;
	vec3 normal = texture(N, TexCoord).xyz;
	float metallic = texture(M, TexCoord).x;
	float roughness = texture(R, TexCoord).x;
	vec3 RColor = texture(RT, TexCoordR).xyz;

	// Gama correction to linear space
	AColor = pow(AColor.xyz, vec3(2.2f));
	RColor = pow(RColor.xyz, vec3(2.2f));

	// Calc normal in world space
	normal = (2.0f * normal) - 1.0f;
	normal = normalize(normal);
	normal = normalize(TangentToWorld * normal);

	// A lineal and depth
	Albedo = vec4(AColor + RColor, 1.0f);

	// Normal World space and metal
	NormalRoughness = vec4(normal.xyz, roughness);

	// World position and metallic
	vec3 Pos = wPosition.xyz;
	PositionMetallic = vec4(Pos, metallic);

	// Position in ligth space
	ShadowPos = ShadowCoord;
}
////////////////////////////////////////////////////////////////////////////////
// Filename: Lum.ps
////////////////////////////////////////////////////////////////////////////////
#version 400


/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec2 TexCoord;


//////////////////////
// OUTPUT VARIABLES //
//////////////////////
layout(location = 0) out vec4 BH;


///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform sampler2D AB;
uniform int ML;


////////////////////////////////////////////////////////////////////////////////
// Shader Functions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
void main()
{
	vec2 OFFSETS[9];
	OFFSETS[0] = vec2(-4, 0);
	OFFSETS[1] = vec2(-3, 0);
	OFFSETS[2] = vec2(-2, 0);
	OFFSETS[3] = vec2(-1, 0);
	OFFSETS[4] = vec2(0, 0);
	OFFSETS[5] = vec2(1, 0);
	OFFSETS[6] = vec2(2, 0);
	OFFSETS[7] = vec2(3, 0);
	OFFSETS[8] = vec2(4, 0);

	float g_BlurWeights[9];
	g_BlurWeights[0] = 0.004815026f;
	g_BlurWeights[1] = 0.028716039f;
	g_BlurWeights[2] = 0.102818575f;
	g_BlurWeights[3] = 0.221024189f;
	g_BlurWeights[4] = 0.28525234f;
	g_BlurWeights[5] = 0.221024189f;
	g_BlurWeights[6] = 0.102818575f;
	g_BlurWeights[7] = 0.028716039f;
	g_BlurWeights[8] = 0.004815026f;

	vec4
	output =  texelFetch(AB, ivec2(gl_FragCoord.xy + OFFSETS[0] * 2), ML) * g_BlurWeights[0];
	output += texelFetch(AB, ivec2(gl_FragCoord.xy + OFFSETS[1] * 2), ML) * g_BlurWeights[1];
	output += texelFetch(AB, ivec2(gl_FragCoord.xy + OFFSETS[2] * 2), ML) * g_BlurWeights[2];
	output += texelFetch(AB, ivec2(gl_FragCoord.xy + OFFSETS[3] * 2), ML) * g_BlurWeights[3];
	output += texelFetch(AB, ivec2(gl_FragCoord.xy + OFFSETS[4] * 2), ML) * g_BlurWeights[4];
	output += texelFetch(AB, ivec2(gl_FragCoord.xy + OFFSETS[5] * 2), ML) * g_BlurWeights[5];
	output += texelFetch(AB, ivec2(gl_FragCoord.xy + OFFSETS[6] * 2), ML) * g_BlurWeights[6];
	output += texelFetch(AB, ivec2(gl_FragCoord.xy + OFFSETS[7] * 2), ML) * g_BlurWeights[7];
	output += texelFetch(AB, ivec2(gl_FragCoord.xy + OFFSETS[8] * 2), ML) * g_BlurWeights[8];

	BH = output;
}
////////////////////////////////////////////////////////////////////////////////
// Filename: TonePS.ps
////////////////////////////////////////////////////////////////////////////////
#version 400

/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec2 TexCoord;


//////////////////////
// OUTPUT VARIABLES //
//////////////////////
layout(location = 0) out vec4 FinalColor;


///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform sampler2D PBR;
uniform sampler2D SSAO;
uniform sampler2D AB;
uniform sampler2D SB;

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader Functions
////////////////////////////////////////////////////////////////////////////////
vec3 Uncharted2Tonemap(vec3 x)
{
	float A = 0.15f;
	float B = 0.5f;
	float C = 0.1f;
	float D = 0.2f;
	float E = 0.02f;
	float F = 0.3f;

	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

vec4 Uncharted2(in vec3 Color, in float exposure)
{
	vec3 W = vec3(11.2f);
	vec3 texColor = Color;
	texColor *= exposure;
	float ExposureBias = 2.0f;
	vec3 curr = Uncharted2Tonemap(ExposureBias * texColor);

	vec3 whiteScale = vec3(1.0f) / Uncharted2Tonemap(W);
	vec3 color = curr * whiteScale;

	vec3 retColor = pow(color, vec3(1.0f / 2.2f));
	return vec4(retColor, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	 vec3 Lw = Uncharted2(texture(PBR, TexCoord).xyz, 1.0f).xyz;

	Lw *= texture(SSAO, TexCoord).xyz;

	Lw += texture(SB, TexCoord).xyz;

	Lw += 0.8f * texture(AB, TexCoord).xyz;

	FinalColor = vec4(Lw.xyz, 1.0f);
}
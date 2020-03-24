////////////////////////////////////////////////////////////////////////////////
// Filename: SSAO.ps
////////////////////////////////////////////////////////////////////////////////
#version 400


/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec2 TexCoord;


//////////////////////
// OUTPUT VARIABLES //
//////////////////////
layout(location = 0) out vec4 SSAO;


///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform sampler2D NR;
uniform sampler2D PM;


////////////////////////////////////////////////////////////////////////////////
// Shader Functions
////////////////////////////////////////////////////////////////////////////////
vec3 GetPosition(in vec2 uv)//el in es para indicar parametro de entrada, out sera para salida
{
	return texture(PM, uv).xyz;
}

vec3 GetNormal(in vec2 uv)//el in es para indicar parametro de entrada, out sera para salida
{
	vec3 DecodeNormal = texture(NR, uv).xyz;

	return normalize(DecodeNormal * 2.0f - 1.0f);
}

vec2 GetRandom(in vec2 uv)//Esta es la distribucion de morntecarlo
{
	float noiseX = (fract(sin(dot(uv, vec2(15.8989f, 76.132f)*1.0f))*46336.23745f));
	float noiseY = (fract(sin(dot(uv, vec2(11.9899f, 62.223f)*2.0f))*34748.34744f));

	return normalize(vec2(noiseX, noiseY));
}

float DoAmbientOcclusion(in vec2 tcoord, in vec2 uv, in vec3 p, in vec3 cnorm)
{
	vec3 diff = GetPosition(tcoord + uv) - p;
	vec3 v = normalize(diff);
	float d = length(diff) * 0.8f;
	return max(0.0f, dot(cnorm, v) - 0.2f)*(1.0 / (1.0 + d)) * 1.2f;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
void main()
{
	vec2 vec[4];
	vec[0] = vec2(1, 0);
	vec[1] = vec2(-1, 0);
	vec[2] = vec2(0, 1);
	vec[3] = vec2(0, -1);

	vec3 p = GetPosition(TexCoord);	

	vec3 n = GetNormal(TexCoord);
	vec2 rand = GetRandom(TexCoord);

	float ao = 0.0f;
	float rad = 0.24f / p.z;

	int iterations = 4;
	for (int j = 0; j < iterations; ++j)
	{
		vec2 coord1 = reflect(vec[j], rand) * rad;
		vec2 coord2 = vec2(coord1.x*0.707 - coord1.y * 0.707,
			coord1.x*0.707 + coord1.y * 0.707);

		ao += DoAmbientOcclusion(TexCoord, coord1 * 0.25, p.xyz, n);
		ao += DoAmbientOcclusion(TexCoord, coord2 * 0.5, p.xyz, n);
		ao += DoAmbientOcclusion(TexCoord, coord1 * 0.75, p.xyz, n);
		ao += DoAmbientOcclusion(TexCoord, coord2, p.xyz, n);
	}

	ao /= 16.0f;

	SSAO.rgb = vec3(1.0f - ao);
	SSAO.w = 1.0f;
}
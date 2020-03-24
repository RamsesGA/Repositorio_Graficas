////////////////////////////////////////////////////////////////////////////////
// Filename: PBRLight.ps
////////////////////////////////////////////////////////////////////////////////
#version 400
#define M_PI 3.14159265359f;

/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec2 TexCoord;


//////////////////////
// OUTPUT VARIABLES //
//////////////////////
layout(location = 0) out vec4 PBRLight;


///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform sampler2D AL;
uniform sampler2D NR;
uniform sampler2D PM;
uniform sampler2D SC;
uniform sampler2DShadow S;
uniform samplerCube EM;
uniform samplerCube IM;

//Camera spot light
uniform float CIC;	// theta
uniform float COC;	// Phi
uniform vec3 CLP;	// position
uniform vec3 CLD;	// Direction
uniform vec4 CLC;	// Color 

//Scenne light
uniform vec3 SLP;	// position
uniform vec3 SLD;	// Direction
uniform vec4 SLC;	// Color 

uniform mat4 SBT;	// sKY bOX TRANSFORM 

//Camera
uniform vec3 VP;

vec2 poissonDisk[16] = vec2[](
	vec2(-0.94201624, -0.39906216),
	vec2(0.94558609, -0.76890725),
	vec2(-0.094184101, -0.92938870),
	vec2(0.34495938, 0.29387760),
	vec2(-0.91588581, 0.45771432),
	vec2(-0.81544232, -0.87912464),
	vec2(-0.38277543, 0.27676845),
	vec2(0.97484398, 0.75648379),
	vec2(0.44323325, -0.97511554),
	vec2(0.53742981, -0.47373420),
	vec2(-0.26496911, -0.41893023),
	vec2(0.79197514, 0.19090188),
	vec2(-0.24188840, 0.99706507),
	vec2(-0.81409955, 0.91437590),
	vec2(0.19984126, 0.78641367),
	vec2(0.14383161, -0.14100790)
	);

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader Functions
////////////////////////////////////////////////////////////////////////////////
float random(vec3 seed, int i) 
{
	vec4 seed4 = vec4(seed, i);
	float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return fract(sin(dot_product) * 43758.5453);
}

vec3 Diffuse(vec3 pA)
{
	return pA / M_PI;
}

float NormalDistribution_GGX(float a, float NdH)
{
	//GGX ISOTROPICO
	float a2 = a * a;
	float NdH2 = NdH * NdH;

	float denominador = NdH2 * (a2 - 1.0f) + 1.0f;
	denominador *= denominador;
	denominador *= M_PI;

	return a2 / denominador;
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL)
{
	//GGX Smith Schick
	float K = a * 0.5f; //Constante de absorcion
	float GV = NdV / (NdV * (1 - K) + K); //arametro de distribucion sobre la vista, lo q percibes segun el angulo de vista
	float GL = NdL / (NdL * (1 - K) + K);//segun lo q percives de rebote

	return GV * GL;
}

vec3 Fresnel_Schlick(vec3 specularColor, vec3 v, vec3 h)
{
	return (specularColor + (1.0f - specularColor) * pow((1.0f - clamp(dot(v, h), 0.0f, 1.0f)), 5));
}

float Specular_D(float a, float NdH) // Distribucion de normales
{
	return NormalDistribution_GGX(a, NdH);
}

float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV) // Valor geometrico 
{
	return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
}

vec3 Specular_F(vec3 specularColor, vec3 v, vec3 h)
{
	return Fresnel_Schlick(specularColor, v, h);
}

vec3 Specular_F_Roughness(vec3 specularColor, float a, vec3 h, vec3 v)
{
	return (specularColor + (max(vec3(1.0f - a, 1.0f - a, 1.0f - a), specularColor) - specularColor) * pow((1.0f - clamp(dot(v, h), 0.0f, 1.0f)), 5));
}

vec3 Specular(vec3 specularColor, vec3 h, vec3 v, vec3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV)//Aporte especular llenando el compute light
{
	float DG = (Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV));
	float Den = (4.0f * NdL * NdV + 0.0001f);

	return (DG * Specular_F(specularColor, v, h)) / Den;
}

vec3 ComputeLight(vec3 albedoColor, vec3 specularColor, vec3 normal, float roughness, vec3 lightColor, vec3 lightDir, vec3 viewDir)
{
	//Calculamos los vectores del brdf
	float NdL = clamp(dot(normal, lightDir), 0.0f, 1.0f);
	float NdV = clamp(dot(normal, viewDir), 0.0f, 1.0f);
	vec3 h = normalize(lightDir + viewDir);
	float NdH = clamp(dot(normal, h), 0.0f, 1.0f);
	float VdH = clamp(dot(viewDir, h), 0.0f, 1.0f);;
	float LdV = clamp(dot(lightDir, viewDir), 0.0f, 1.0f);
	float a = max(0.001f, roughness * roughness);

	vec3 ColorDiffuse = Diffuse(albedoColor);
	vec3 ColorSpecular = Specular(specularColor, h, viewDir, lightDir, a, NdL, NdV, NdH, VdH, LdV);

	return lightColor * NdL * (ColorDiffuse * (1.0f - ColorSpecular) + ColorSpecular);
}
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Take textures samplers
	vec3 albedoColor = texture(AL, TexCoord).xyz;
	vec3 normal = texture(NR, TexCoord).xyz;
	vec3 pos = texture(PM, TexCoord).xyz;
	float metallic = texture(PM, TexCoord).w;
	float roughness = 1.0f-texture(NR, TexCoord).w;
	vec4 ShadowCoord = texture(SC, TexCoord);

	// calc real A and spec color
	vec3 realA = (albedoColor - (albedoColor * metallic));
	vec3 realSpecularColor = mix(vec3(0.03f, 0.03f, 0.03f), albedoColor, vec3(metallic));

	//Calc View direction
	vec3 viewDir = normalize(VP.xyz - pos.xyz);
		
	//Shadow Calc------------------------------------------------	
	float visibility = 1.0;

	// Fixed bias
	float bias = 0.005;

	// Sample the shadow map 4 times
	for (int i = 0; i<4; i++)
	{
		int index = i;
		
		visibility -= 0.2*(1.0 - texture(S, vec3(ShadowCoord.xy + poissonDisk[index] / 700.0, (ShadowCoord.z - bias) / ShadowCoord.w)));
	}

	//Irradiance, environment & fresnel
	vec4 reflectVector = SBT * vec4(reflect(-viewDir, normal), 1.0f);
	vec4 NormalSBT = SBT * vec4(normal, 1.0f);

	float mipIndex = roughness * roughness * 8.0f;//valor q se controlara del rougnes del material
	vec3 envColor = textureLod(EM, reflectVector.xyz, mipIndex).xyz;//texCUBElod usa el valor w de vector
	vec3 irradiance = texture(IM, NormalSBT.xyz).xyz;
	envColor = pow(envColor.rgb, vec3(2.2f));
	vec3 envFresnel = Specular_F_Roughness(realSpecularColor, roughness * roughness, normal, viewDir);

	vec3 Ambient = (envFresnel * realA * envColor) + (envFresnel * realA * irradiance) * 0.003;

	// scene SpotLight Calc----------------------------------------
	// Calc pos view and light dir per pixel	
	vec3 lightDir = normalize(SLD);
	
	// Calculate light color		
	vec3 light1 = visibility * ComputeLight(realA, realSpecularColor, normal, roughness, SLC.xyz, lightDir, viewDir) + Ambient;

	//// camera SpotLight Calc----------------------------------------
	// Calc pos view and light dir per pixel	
	lightDir = normalize(CLP.xyz - pos.xyz);

	// Calculate light color		
	vec3 light2 = (ComputeLight(realA, realSpecularColor, normal, roughness, CLC.xyz, lightDir, viewDir) * 2.0f) + Ambient;
	
	//Calculate the intensity of the light in pixel in spotligth1 
	float lightIntensity = (dot(-lightDir, CLD) - cos(COC / 2.0f)) / (cos(CIC / 3.0f) - cos(COC / 2.0f));
	vec3 Spot2 = clamp((CLC * lightIntensity), 0.0f, 1.0f).xyz;
	
	// PBR output
	PBRLight = vec4((light1) + (Spot2 * light2), 1.0f);
}
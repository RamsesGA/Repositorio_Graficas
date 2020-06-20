////////////////////////////////////////////////////////////////////////////////
// Filename: SkyBox.ps
////////////////////////////////////////////////////////////////////////////////
#version 400


/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec3 SkyBoxCoord;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
layout(location = 0) out vec4 SkyBox;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform samplerCube EM;


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
void main()
{
	SkyBox = vec4(texture(EM, SkyBoxCoord).xyz, 1.0f);
}
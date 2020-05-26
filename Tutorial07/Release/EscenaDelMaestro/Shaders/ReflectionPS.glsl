////////////////////////////////////////////////////////////////////////////////
// Filename: Reflection.ps
////////////////////////////////////////////////////////////////////////////////
#version 400

/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec2 TexCoord;


//////////////////////
// OUTPUT VARIABLES //
//////////////////////
layout(location = 0) out vec4 Color;


///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform sampler2D RT;


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Take textures samplers
	Color = texture(RT, TexCoord);
}
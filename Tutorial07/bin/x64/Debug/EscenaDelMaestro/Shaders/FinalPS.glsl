////////////////////////////////////////////////////////////////////////////////
// Filename: Final.ps
////////////////////////////////////////////////////////////////////////////////
#version 400


/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec2 TexCoord;


//////////////////////
// OUTPUT VARIABLES //
//////////////////////
out vec4 FinalColor;


///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform sampler2D F;


////////////////////////////////////////////////////////////////////////////////
// Shader Functions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
void main()
{
	FinalColor = texture(F, TexCoord);
}
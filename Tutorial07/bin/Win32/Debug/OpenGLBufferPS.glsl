////////////////////////////////////////////////////////////////////////////////
// Filename: OpenGLBufferPS.ps
////////////////////////////////////////////////////////////////////////////////
#version 400

in vec3 outNormal;
in vec2 TexCoord;

uniform sampler2D textureDifuse;
uniform mat4 WordMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec4 LightDir;

layout(location = 0) out vec3 color;

void main() 
{
    vec4 light = normalize(-LightDir);

    float NormDirLight = dot(outNormal, light.xyz);

	// Inverted the TeexCoords y
	vec2 texC = vec2(TexCoord.x, -TexCoord.y);
	
	color = texture(textureDifuse, texC).rgb * NormDirLight;
}
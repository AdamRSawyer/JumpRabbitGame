#version 330 core

// Interpolated values from the vertex shaders
in vec2 uvCoord_fs;

uniform sampler2D diffuseTexture;

// Ouput data
out vec3 color;

void main(){

	// Output color = color specified in the vertex shader, 
	// interpolated between all 3 surrounding vertices
	color = texture(diffuseTexture, uvCoord_fs.xy).rgb;
	//color = texture(diffuseTexture, uvCoord_fs.xy);

}
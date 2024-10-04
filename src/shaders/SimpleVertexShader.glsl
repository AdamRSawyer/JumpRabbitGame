#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 uvCoord;
layout(location = 2) in vec3 norm;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

// Output data: Will be sent to fragment shader
out vec2 uvCoord_fs;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	uvCoord_fs = uvCoord;
}


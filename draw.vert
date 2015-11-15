#version 150

in vec3 inPosition;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

//out position; 

void main(void){
	//position = vec3(modelViewMatrix * vec4(inPosition, 1.0));
	gl_Position = projectionMatrix * modelViewMatrix * vec4(inPosition, 1.0);
}

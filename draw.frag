#version 330

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 outColor;

void main(void){
	if(texture(texUnit, outTexCoord).g >= 0)
		outColor = texture(texUnit, outTexCoord);
	else	
		outColor = vec4(1.0, 0.0, 0.0, 1.0);
}

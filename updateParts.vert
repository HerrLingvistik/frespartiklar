#version 330

in vec3 in_Position;

out vec2 outTexCoord;



void main(void)
{
	//hitta position med hjälp av texturen
	outTexCoord = in_Position.xy/2.0 + vec2(0.5,0.5);
	
}

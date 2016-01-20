#version 330

in vec3 in_Position;

uniform sampler2D texUnit;

out vec2 outTexCoord;

const float worldSize = 255.0;

void main(void)
{
	vec2 texPos = in_Position.xy + vec2(0.5,0.5);

	outTexCoord = in_Position.xy + vec2(0.5,0.5);
	
	vec2 p = texture(texUnit, texPos).rg;

	//gl_Position = vec4(p/worldSize * 2.0 - 1.0, 0, 1);	
	//gl_PointSize = 10.0;

	if(in_Position.y < 0)
		gl_Position = vec4(in_Position.xy, 0, 1);
	else	
		gl_Position = vec4(p/worldSize, 0, 1);
}

#version 140

out vec4 fragColor;

in vec2 coord;

uniform sampler2D source;

void main()
{
	vec3 color = vec3(0.0);
	float offset = 1.0 / 512;
	
	for (int i = -20; i <= 20; ++i)
	{
		color += texture(source, coord + vec2(i * offset, 0.0)).xyz;
	}
	
	fragColor = vec4(color / 41.0, 1.0);
}

#version 140

out vec4 fragColor;

in vec2 v_uv;

uniform float size;
uniform sampler2D source;

void main()
{
	vec3 color = vec3(0.0);
        float offset = 1.0 / size;
	
	for (int i = -20; i <= 20; ++i)
	{
		color += texture(source, v_uv + vec2(0.0, i * offset)).xyz;
	}
	
	fragColor = vec4(color / 41.0, 1.0);
}

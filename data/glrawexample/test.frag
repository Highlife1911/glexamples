#version 140

uniform sampler2D source;

layout (location = 0) out vec4 fragColor;

in vec2 coord;

void main()
{
	fragColor = vec4(texture(source, coord).xyz, 1.0);
}
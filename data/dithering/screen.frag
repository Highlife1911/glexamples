#version 140

uniform sampler2D source;
uniform vec2 pos;
uniform vec2 size;
uniform float zoom;

out vec4 fragColor;

in vec2 v_uv;

void main()
{
	ivec2 img_size = textureSize(source, 0);
	vec2 posN = pos/size;
	
	vec2 offset = (size-img_size*zoom)/(2*size);
	vec2 pos_abs = (v_uv-offset-posN*zoom)*size/img_size/zoom;
	fragColor = vec4(texture(source, pos_abs).rgb, 1.f);
}
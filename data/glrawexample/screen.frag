#version 140

uniform sampler2D source;
uniform vec2 pos;
uniform vec2 size;
uniform float zoom;

out vec4 fragColor;

in vec2 v_uv;

void main()
{
	vec2 posN = pos/size;
	ivec2 img_size = textureSize(source, 0);
	vec2 offset = vec2((size.x - img_size.x*zoom)/(size.x*2),(size.y - img_size.y*zoom)/(size.y*2));
	if(v_uv.x <= offset.x + posN.x || v_uv.x >= 1.f - offset.x + posN.x || v_uv.y <= offset.y + posN.y || v_uv.y >= 1.f - offset.y + posN.y)
		discard;
	vec3 color = texture(source, (v_uv-offset-posN)*size/img_size/zoom).xyz; 
	fragColor = vec4(color,1.f);
}
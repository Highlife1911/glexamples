#version 420

out vec4 fragColor;

layout(rgba8) uniform image2D source;
uniform int chunk_size;
uniform int num_colors;
uniform vec3 grey;


float find_color( float color )
{
	int x = int(color * (num_colors-1) + 0.5);
	float c = float(x);
	return c/(num_colors-1);
}

float greyscale( vec3 data )
{
	return clamp( (grey.r*data.r) + (grey.g*data.g) + (grey.b*data.b), 0.0, 1.0 );
}

void add_color( float add, ivec2 base, ivec2 pos )
{
	//prevent writing over borders
	float over_x = step( pos.x+1, base.x + chunk_size );
	float over_y = step( pos.y+1, base.y + chunk_size );
	float factor = min( over_x, over_y );	

	vec3 data = imageLoad(source, pos).xyz + vec3( factor * add );
	imageStore(source, pos, vec4(data, 1.0));
}

void main()
{
	int base_x = int(gl_FragCoord.x * chunk_size - chunk_size/2);
	int base_y = int(gl_FragCoord.y * chunk_size - chunk_size/2);
	ivec2 pos = ivec2(base_x, base_y);
	for( int y = 0; y < chunk_size; y++ )
	{
		for( int x = 0; x < chunk_size; x++ )
		{
			ivec2 act_pos = pos + ivec2(x,y);
			float data = greyscale( imageLoad(source, act_pos).xyz );
			float c = find_color( data );
		
			//update current
			vec3 md = vec3( c );
			imageStore(source, act_pos, vec4(md, 1.0));
		
			float diff = (data - c) / 16;
			add_color( diff * 7, pos, act_pos + ivec2( 1,0) );
			add_color( diff * 3, pos, act_pos + ivec2(-1,1) );
			add_color( diff * 5, pos, act_pos + ivec2( 0,1) );
			add_color( diff * 1, pos, act_pos + ivec2( 1,1) );
		}
	}

	fragColor = vec4(1.0);
}

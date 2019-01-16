#version 450 compatibility

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;


in vec3 geom_pos[3];
in vec3 geom_normal[3];
in vec2 geom_tex[3];
in float geom_split[3];

out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_tex;
out vec3 tan;
out vec3 bitan;

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

void main()
{
	
	mat2x3 E = mat2x3(geom_pos[1].xyz-geom_pos[0].xyz, geom_pos[2].xyz-geom_pos[0].xyz);
	mat2 UV = mat2(geom_tex[1]-geom_tex[0], geom_tex[2]-geom_tex[0]);
	mat2x3 M = transpose(inverse(transpose(UV)) * transpose(E));
	tan = M[0];
	bitan = M[1];

	vec3 tri_nor = (geom_normal[0] + geom_normal[1] + geom_normal[2])/3;
	mat4 move = translate(geom_split[0] * tri_nor.x, geom_split[0]* tri_nor.y, geom_split[0] * tri_nor.z);
	for(int i = 0; i < gl_in.length(); i++)
	{
		frag_normal = geom_normal[i];
		frag_tex = geom_tex[i];
		
		if(geom_split[i] > 0.0f)
		{
			gl_Position = move * gl_in[i].gl_Position, 1.0;
			frag_pos = (move * vec4(geom_pos[i], 1.0)).xyz;
		}
		else
		{
			gl_Position = gl_in[i].gl_Position;
			frag_pos = geom_pos[i];
		}
		EmitVertex();
	}



	EndPrimitive();
}
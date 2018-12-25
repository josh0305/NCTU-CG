#version 450 compatibility

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;


in vec3 geom_pos[3];
in vec3 geom_normal[3];
in vec2 geom_tex[3];

out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_tex;
out vec3 tan;
out vec3 bitan;


void main()
{
	
	mat2x3 E = mat2x3(geom_pos[1].xyz-geom_pos[0].xyz, geom_pos[2].xyz-geom_pos[0].xyz);
	mat2 UV = mat2(geom_tex[1]-geom_tex[0], geom_tex[2]-geom_tex[0]);
	mat2x3 M = transpose(inverse(UV) * transpose(E));
	tan = M[0];
	bitan = M[1];

	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = gl_in[i].gl_Position;
		frag_normal = geom_normal[i];
		frag_pos = geom_pos[i];
		frag_tex = geom_tex[i];
		EmitVertex();
	}
	EndPrimitive();
}
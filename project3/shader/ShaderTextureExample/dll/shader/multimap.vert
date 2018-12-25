#version 450 compatibility

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoor;

out vec3 geom_pos;
out vec3 geom_normal;
out vec2 geom_tex;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform mat4 Revolve;
uniform mat4 Tilt;


void main() {
	gl_Position = Projection * ModelView * Tilt * Revolve * vec4(pos, 1.0);
	geom_pos = (ModelView * Tilt * Revolve * vec4(pos,1.0)).xyz;
	geom_normal = (Tilt * Revolve * vec4(normal,1.0)).xyz;
	geom_tex = texcoor;
}
#version 450 compatibility

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoor;
layout(location = 3) in vec3 trinor;

out vec3 geom_pos;
out vec3 geom_normal;
out vec2 geom_tex;

out float geom_split;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform float translate_x;

uniform float splited;

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

void main() {
	geom_split = splited;
	gl_Position = Projection * ModelView * translate(translate_x,0.0,0.0) * vec4(pos, 1.0);
	geom_pos = (ModelView * translate(translate_x,0.0,0.0) * vec4(pos,1.0)).xyz;
	geom_normal = vec4(normal,1.0).xyz;
	geom_tex = texcoor;
}
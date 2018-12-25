#version 450 compatibility

layout(binding = 0) uniform sampler2D Earth_tex;
layout(binding = 1) uniform sampler2D Earth_nor;
layout(binding = 2) uniform sampler2D Earth_spec;

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex;
in vec3 tan;
in vec3 bitan;

uniform vec3 Control;
uniform vec3 Camera;
uniform mat4 MVM;
uniform vec3 Light;

out vec4 outColor;

void main(){
	vec4 am, di, sp;

	vec3 N;
	if(Control.y > 0)
	{
		vec3 color = texture2D(Earth_nor, frag_tex).rgb*2 - vec3(1.0f, 1.0f, 1.0f);
		N = normalize(color.x * tan + color.y * bitan + color.z * frag_normal);
	}
	else
	{
		N = normalize(frag_normal);
	}
	
	vec3 camLoc = (MVM * vec4(Camera, 1.0f)).xyz;
	vec3 V = normalize(camLoc - frag_pos);

	vec3 L = normalize(gl_LightSource[0].position.xyz - frag_pos);
	//vec3 LightPos = (MVM * vec4(Light, 1.0f)).xyz;
	//vec3 L = normalize( LightPos - frag_pos);
	vec3 R = reflect(-L, N);

	//ambient = Ia * Ka
	am = vec4(0.7, 0.7, 0.7, 0.1) * 0.1;

	//diffuse = Id * Kd * (N*L)
	vec4 difcolor;
	if(Control.x > 0)
		difcolor = texture2D(Earth_tex, frag_tex);
	else
		difcolor = vec4(0.35, 0.3, 0.15, 1.0);
	di =  difcolor * 1 * max(dot(N, L), 0.0f);

	//specular = Is * Ks * (R * V)^ns
	vec4 speccolor;
	if(Control.z > 0)
		speccolor = texture2D(Earth_spec, frag_tex);
	else
		speccolor = vec4(1.0, 1.0, 1.0, 1.0);
	sp = speccolor * 0.3 * pow(max(dot(R, V), 0.0f), 32);

	outColor = am + di + sp;
}
#version 450 compatibility

layout(binding = 0) uniform sampler2D moon_tex;
layout(binding = 1) uniform sampler2D moon_nor;

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_tex;
in vec3 tan;
in vec3 bitan;

out vec4 outColor;

void main(){
	vec4 am, di, sp;

	
	vec3 color = texture2D(moon_nor, frag_tex).rgb*2 - vec3(1.0f, 1.0f, 1.0f);
	vec3 N = normalize(color.x * tan + color.y * bitan + color.z * frag_normal);
	
	vec3 V = normalize(-frag_pos);
	vec3 L = normalize(gl_LightSource[0].position.xyz - frag_pos);
	vec3 R = reflect(-L, N);

	//ambient = Ia * Ka
	am = vec4(0.7, 0.7, 0.7, 0.1) * 0.1;

	//diffuse = Id * Kd * (N*L)
	di =  texture2D(moon_tex, frag_tex) * 1 * max(dot(N, L), 0.0f);

	//specular = Is * Ks * (R * V)^ns
	vec4 speccolor = vec4(1.0, 1.0, 1.0, 1.0);
	sp = speccolor * 0.3 * pow(max(dot(R, V), 0.0f), 32);

	outColor = am + di + sp;
}
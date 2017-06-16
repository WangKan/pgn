#include "CommonVS3D.h"

uniform EnvCBlock@0 // enum BindingPoint
{
	mat4 viewProj;
	mat3x4 view;
	vec3 camPos;
};

flat out lowp float Ks;
flat out lowp float shininess;

void main()
{
	vec3 pos = position * instances[gl_InstanceID].scale.xyx;

	mat3x4 world = instances[gl_InstanceID].world;

	vec3 wpos = vec4(pos, 1.0) * world;
	vec3 wnor = vec4(normal, 0.0) * world;

	gl_Position = vec4(wpos, 1.0) * viewProj;
	n = (vec4(wnor, 0.0) * view + 1.0) * 0.5;

	tc0 = texCoord0;
	Ks = materials[materialIndex].Ks;
	shininess = materials[materialIndex].shininess / 100.0;
}

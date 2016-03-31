#include "commonVS3D.h"

uniform EnvCBlock@0 // enum BindingPoint
{
	vec3 camPos;
	mat3x4 view;
	mat4 viewProj;
};

flat out lowp float Ks;
flat out lowp float shininess;

void main()
{
	vec3 pos = position * instances[gl_InstanceID].scale.xyx;

	mat3x4 world = instances[gl_InstanceID].world;

	world[0][3] -= camPos.x;
	world[1][3] -= camPos.y;
	world[2][3] -= camPos.z;

	vec3 wpos = vec4(pos, 1.0) * world;
	vec3 wnor = vec4(normal, 0.0) * world;

	gl_Position = vec4(wpos, 1.0) * viewProj;
	n = (vec4(wnor, 0.0) * view + 1.0) * 0.5;

	tc0 = texCoord0;
	Ks = materials[materialIndex].Ks;
	shininess = materials[materialIndex].shininess / 100.0;
}

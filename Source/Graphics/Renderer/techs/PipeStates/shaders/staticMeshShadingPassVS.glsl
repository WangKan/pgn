#include "CommonVS3D.h"
#include "ForwardShadingPassEnvCBlock.h"

out vec3 wpos;
flat out lowp float Ks;
flat out lowp float shininess;

void main()
{
	vec3 pos = position * instances[gl_InstanceID].scale.xyx;

	mat3x4 world = instances[gl_InstanceID].world;

	wpos = vec4(pos, 1.0) * world;
	n = vec4(normal, 0.0) * world;

	gl_Position = vec4(wpos, 1.0) * viewProj;

	tc0 = texCoord0;
	Ks = materials[materialIndex].Ks;
	shininess = materials[materialIndex].shininess;
}

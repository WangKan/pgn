#include "CommonVS3D.h"

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
	vec3 mpos = vec3(0.0, 0.0, 0.0);
	vec3 mnor = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < numAssignedBones; i++)
	{
		mpos += vec4(position, 1.0) * model[boneIndices[i]] * boneWeights[i];
		mnor += vec4(normal, 0.0) * model[boneIndices[i]] * boneWeights[i];
	}

	mpos *= instances[gl_InstanceID].scale.xyx;

	mat3x4 world = instances[gl_InstanceID].world;

	vec3 wpos = vec4(mpos, 1.0) * world;
	vec3 wnor = vec4(mnor, 0.0) * world;

	gl_Position = vec4(wpos, 1.0) * viewProj;
	n = (normalize(vec4(wnor, 0.0) * view) + 1.0) * 0.5;

	tc0 = texCoord0;
	Ks = materials[materialIndex].Ks;
	shininess = materials[materialIndex].shininess / 100.0;
}

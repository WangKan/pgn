#include "CommonVS3D.h"
#include "ForwardShadingPassEnvCBlock.h"

out vec3 wpos;
flat out lowp float Ks;
flat out mediump float shininess;

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

	wpos = vec4(mpos, 1.0) * world;
	n = vec4(mnor, 0.0) * world;

	gl_Position = vec4(wpos, 1.0) * viewProj;

	tc0 = texCoord0;
	Ks = materials[materialIndex].Ks;
	shininess = materials[materialIndex].shininess;
}

#include "CommonVS3D.h"
#include "ZOnlyPassEnvCBlock.h"

void main()
{
	vec3 mpos = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < numAssignedBones; i++)
	{
		mpos += vec4(position, 1.0) * model[boneIndices[i]] * boneWeights[i];
	}

	mpos *= instances[gl_InstanceID].scale.xyx;

	mat3x4 world = instances[gl_InstanceID].world;

	vec3 wpos = vec4(mpos, 1.0) * world;

	gl_Position = vec4(wpos, 1.0) * viewProj;
}

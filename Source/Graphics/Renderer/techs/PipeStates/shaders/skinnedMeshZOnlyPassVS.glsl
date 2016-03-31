#include "CommonVS3D.h"

uniform EnvCBlock@0 // enum BindingPoint
{
	vec3 camPos;
	mat4 viewProj;
};

void main()
{
	vec3 mpos = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < numAssignedBones; i++)
	{
		mpos += vec4(position, 1.0) * model[boneIndices[i]] * boneWeights[i];
	}

	mpos *= instances[gl_InstanceID].scale.xyx;

	mat3x4 world = instances[gl_InstanceID].world;

	world[0][3] -= camPos.x;
	world[1][3] -= camPos.y;
	world[2][3] -= camPos.z;

	vec3 wpos = vec4(mpos, 1.0) * world;

	gl_Position = vec4(wpos, 1.0) * viewProj;
}

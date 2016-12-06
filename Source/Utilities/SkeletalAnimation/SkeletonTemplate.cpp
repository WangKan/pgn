#include <cstring>
#include "SkeletonTemplate.h"

SkeletonTemplate::SkeletonTemplate(int numBones, pgn::Float4x3* offsetMats, pgn::Float4 defaultRot[], pgn::Float3 defaultPos[], unsigned char parentIndices[], pgn::Pool* pool, BoneAllocator& boneAllocator)
	: bones(boneAllocator)
{
	this->pool = pool;

	for (int i = 0; i < numBones; i++)
	{
		bones.emplace_back();
		Bone* bone = &bones.back();
		bone->offsetMat.float4x3 = offsetMats[i];
		pgn::Float4& col3 = bone->offsetMat[3];
		col3[0] = 0.0f;
		col3[1] = 0.0f;
		col3[2] = 0.0f;
		col3[3] = 1.0f;
		bone->defaultRot = defaultRot[i];
		bone->defaultPos = defaultPos[i];
		bone->parentIndex = parentIndices[i];
	}
}

void SkeletonTemplate::dispose()
{
}

int SkeletonTemplate::getNumBones()
{
	return (int)bones.size();
}

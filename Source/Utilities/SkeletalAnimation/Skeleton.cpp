#include <PGN/Math/Math.h>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#undef PGN_STATIC_IMPORT
#include <string.h>
#include "Anim.h"
#include "Animation.h"
#include "Skeleton.h"
#include "SkeletonTemplate.h"

Skeleton::Skeleton(pgn::Pool* pool)
{
	this->pool = pool;
	numAnimLayers = 0;
}

void Skeleton::dispose()
{

}

void Skeleton::playAnimation(pgn::Animation* anim, int transitionTime, pgn::TransitionFunc transitionFunc, float rate, int startTime)
{
	assert(numAnimLayers < maxNumAnimLayers);
	AnimLayer* layer = &animLayers[numAnimLayers++];
	layer->anim = ((Animation*)anim)->anim;
	layer->transitionTime = transitionTime;
	layer->transitionFunc = transitionFunc;
	layer->rate = rate;
	layer->startTime = startTime;

	layer->time = 0;
}

void Skeleton::updatePose(int dt, pgn::SkeletonTemplate* _templ, pgn::Float4x3* boneMats)
{
	if (dt)
	{
		for (int i = 0; i < numAnimLayers; i++)
			animLayers[i].time += dt;
	}

	if (boneMats)
	{
		const int maxNumBones = 128;

		pgn::Float4 quat[maxNumBones];
		pgn::Float3 pos[maxNumBones];
		unsigned char layerMasksRot[maxNumBones];
		unsigned char layerMasksPos[maxNumBones];

		assert(sizeof(layerMasksRot[0]) * 8 == maxNumAnimLayers);
		assert(sizeof(layerMasksPos[0]) * 8 == maxNumAnimLayers);

		SkeletonTemplate* templ = (SkeletonTemplate*)_templ;
		size_t numBones = templ->bones.size();

		memset(layerMasksRot, 0, sizeof(layerMasksRot[0]) * numBones);
		memset(layerMasksPos, 0, sizeof(layerMasksPos[0]) * numBones);

		for (int i = 0; i < numAnimLayers; i++)
		{
			AnimLayer* layer = &animLayers[i];
			Anim* anim = (Anim*)layer->anim->core();

			if (!anim)
			{
				layerMasksRot[0] |= 1 << i; // 防止在动画还没加载好的时候被认为是不活跃的层。
				continue;
			}

			int dt = (int)(layer->time * layer->rate);
			int time = layer->startTime + dt;
			int duration = anim->duration;
			time = (time % duration + duration) % duration;

			float weight;

			switch (layer->transitionFunc)
			{
			case pgn::linear:
				weight = dt >= layer->transitionTime ? 1.0f : dt / (float)layer->transitionTime;
				break;
			case pgn::easeIn:
			case pgn::easeOut:
			case pgn::easeInOut:
                weight = 0.0f;
				;
			}

			auto itBoneAnim = anim->boneAnims.begin();
			for (size_t j = 0; j < numBones; j++)
			{
				BoneAnim* boneAnim = &*itBoneAnim;

				auto& rotKeyMap = boneAnim->rotKeyMap;
				if (!rotKeyMap.empty())
				{
					auto it = rotKeyMap.upper_bound(time);
					int t1 = it->first;
					pgn::Float4* b = &it->second;

					it--;
					int t0 = it->first;
					pgn::Float4* a = &it->second;

					pgn::Float4 r;
					pgn::slerp(a, b, (time - t0) / (float)(t1 - t0), &r);

					if (layerMasksRot[j] && weight != 1.0f)
					{
						pgn::slerp(&quat[j], &r, weight, &quat[j]);
						layerMasksRot[j] |= 1 << i;
					}
					else
					{
						quat[j] = r;
						layerMasksRot[j] = 1 << i;
					}
				}

				auto& posKeyMap = boneAnim->posKeyMap;
				if (!posKeyMap.empty())
				{
					auto it = posKeyMap.upper_bound(time);
					int t1 = it->first;
					pgn::Float3* b = &it->second;

					it--;
					int t0 = it->first;
					pgn::Float3* a = &it->second;

					pgn::Float3 r;
					pgn::lerp(a, b, (time - t0) / (float)(t1 - t0), &r);

					if (layerMasksPos[j] && weight != 1.0f)
					{
						pgn::lerp(&pos[j], &r, weight, &pos[j]);
						layerMasksPos[j] |= 1 << i;
					}
					else
					{
						pos[j] = r;
						layerMasksPos[j] = 1 << i;
					}
				}

				itBoneAnim++;
			}
		}

		static pgn::Float4x3 I =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
		};

		pgn::Float4x3 combinedMats[maxNumBones];
		combinedMats[0] = I;

		auto itBone = templ->bones.begin();
		itBone++;
		for (size_t i = 1; i < numBones; i++)
		{
			Bone* bone = &*itBone;

			if (!layerMasksRot[i])
				quat[i] = bone->defaultRot;

			if (!layerMasksPos[i])
				pos[i] = bone->defaultPos;

			pgn::Float4x4 localMat;

			pgn::quatToMat(&quat[i], &localMat.float4x3);

			localMat[0][3] = pos[i].x;
			localMat[1][3] = pos[i].y;
			localMat[2][3] = pos[i].z;

			localMat[3][0] = 0.0f;
			localMat[3][1] = 0.0f;
			localMat[3][2] = 0.0f;
			localMat[3][3] = 1.0f;

			pgn::mul(&localMat, &combinedMats[bone->parentIndex], &combinedMats[i]);
			pgn::mul(&bone->offsetMat, &combinedMats[i], &boneMats[i]);

			itBone++;
		}

		unsigned short activeLayerMask = 0;
		for (size_t i = 0; i < numBones; i++)
		{
			activeLayerMask |= layerMasksRot[i];
			activeLayerMask |= layerMasksPos[i];
		}

		int numActiveLayers = 0;
		for (int i = 0; i < numAnimLayers; i++)
		{
			if (activeLayerMask & (1 << i))
			{
				if (numActiveLayers != i)
					animLayers[numActiveLayers] = animLayers[i];

				numActiveLayers++;
			}
		}
		numAnimLayers = numActiveLayers;
	}
}

void Skeleton::stopAll()
{
	numAnimLayers = 0;
}

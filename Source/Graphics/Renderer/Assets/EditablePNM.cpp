#include <float.h>
#include "../EditableGeometry.h"
#include "EditablePNM.h"

EditablePNM::EditablePNM(pgn::AssetFactory* factory, EditableGeometry* geom, pgn::SkeletonTemplateFactory* skeletonFactory)
	: PNM(factory, geom, skeletonFactory)
{
}

bool EditablePNM::cook(void* rawData)
{
	PNM::cook(rawData);
	return true;
}

class Aabb
{
public:
	pgn::Float3 min;
	pgn::Float3 max;

	void add(float* v)
	{
        if(v[0] < min.x) min.x = v[0];
        if(v[1] < min.y) min.y = v[1];
        if(v[2] < min.z) min.z = v[2];
        
        if(v[0] > max.x) max.x = v[0];
        if(v[1] > max.y) max.y = v[1];
        if(v[2] > max.z) max.z = v[2];
	}
};

bool EditablePNM::submit(void* rawData, void* customArg)
{
	pgn::PNMHeader* header = (pgn::PNMHeader*)rawData;
	float (*pos)[3] = (float(*)[3]) ((char*)header + header->attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_POSITION]);
	unsigned short* indices = (unsigned short*) ((char*)header + header->indicesChunkOffset);

	for (int i = 0; i < header->numSubsets; i++)
	{
		Aabb aabb = { { FLT_MAX, FLT_MAX, FLT_MAX }, { -FLT_MAX, -FLT_MAX, -FLT_MAX } };

		int numIndices = header->numIndices[i];

		for (int j = 0; j < numIndices; j++)
			aabb.add(pos[indices[j]]);

		indices += numIndices;

		EditableGeometry* editableGeom = (EditableGeometry*)geom;
		editableGeom->subsets[i].aabb.min = aabb.min;
		editableGeom->subsets[i].aabb.max = aabb.max;
	}

	PNM::submit(rawData, customArg);

	return true;
}

void EditablePNM::unload(void* customArg)
{
	PNM::unload(customArg);
}

void* EditablePNM::core()
{
	return ready ? geom : 0;
}

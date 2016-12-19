#include <math.h>
#include "../EditableGeometry.h"
#include "EditableMesh.h"

EditableMesh::EditableMesh(pgn::AssetFactory* factory, EditableGeometry* geom, pgn::SkeletonTemplateFactory* skeletonFactory)
	: Mesh(factory, geom, skeletonFactory)
{
}

bool EditableMesh::cook(void* rawData)
{
	Mesh::cook(rawData);
	return true;
}

class Aabb
{
public:
	pgn::Float3 min;
	pgn::Float3 max;

	void add(float* v)
	{
		for (int i = 0; i < 3; i++)
			if (v[i] < min[i])
				min[i] = v[i];

		for (int i = 0; i < 3; i++)
			if (v[i] > max[i])
				max[i] = v[i];
	}
};

bool EditableMesh::submit(void* rawData, void* customArg)
{
	pgn::PNMHeader* header = (pgn::PNMHeader*)rawData;
	float (*pos)[3] = (float(*)[3]) ((char*)header + header->attribChunkOffsetTable[pgn::PNMHeader::ATTRIB_POSITION]);
	unsigned short* indices = (unsigned short*) ((char*)header + header->indicesChunkOffset);

	for (int i = 0; i < header->numSubsets; i++)
	{
		Aabb aabb = { { FLT_MAX, FLT_MAX, FLT_MAX }, { FLT_MIN, FLT_MIN, FLT_MIN } };

		int numIndices = header->numIndices[i];

		for (int j = 0; j < numIndices; j++)
			aabb.add(pos[indices[j]]);

		indices += numIndices;

		EditableGeometry* editableGeom = (EditableGeometry*)geom;
		editableGeom->subsets[i].aabb.min = aabb.min;
		editableGeom->subsets[i].aabb.max = aabb.max;
	}

	Mesh::submit(rawData, customArg);

	return true;
}

void EditableMesh::unload(void* customArg)
{
	Mesh::unload(customArg);
}

void* EditableMesh::core()
{
	return ready ? geom : 0;
}

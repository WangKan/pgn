#include <PGN/Assets/NAV.h>
#include <PGN/FileStream/FileStream.h>
#include <PGN/Math/Math.h>
#include <PGN/RenderingSystem/Buffer.h>
#include <PGN/RenderingSystem/RenderingSystem.h>
#include <PGN/RenderingSystem/VertexInputLayout.h>
#include <PGN/Utilities/CircularBuffer.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplate.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplateFactory.h>
#include "../GeometryHelper.h"
#include "../NavGeometry.h"
#include "NAV.h"

NAV::NAV(pgn::AssetFactory* factory, NavGeometry* geom)
	: Asset(factory)
{
	this->geom = geom;
	ready = false;
}

bool NAV::cook(void* rawData)
{
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

bool NAV::submit(void* rawData, void* customArg)
{
	pgn::RenderingSystem* rs = (pgn::RenderingSystem*)customArg;
	pgn::NAVHeader* header = (pgn::NAVHeader*)rawData;

	pgn::VertexAttribDesc attribDescs[] = { "position", pgn::FLOAT3, 0 };
	void* attribs[] = { (char*)header + header->verticesChunkOffset };
	int numIndices[] = { header->numIndices };

	GeometryHelper helper(rs);
	helper.createGeometry(geom, 1, attribDescs, header->numVertices, 1, numIndices, attribs, (char*)header + header->indicesChunkOffset);
	geom->primType = pgn::PrimType::TRIANGLE_LIST;
	geom->brdfCoeffBuf.buf = 0;

	rs->flush();

	Aabb aabb = { { FLT_MAX, FLT_MAX, FLT_MAX }, { FLT_MIN, FLT_MIN, FLT_MIN } };
	float(*pos)[3] = (float(*)[3])attribs[0];

	for (int i = 0; i < header->numVertices; i++)
		aabb.add(pos[i]);

	geom->aabb.min = aabb.min;
	geom->aabb.max = aabb.max;

	ready = true;

	return true;
}

void NAV::unload(void* customArg)
{
	if (!ready)
		return;

	pgn::RenderingSystem* rs = (pgn::RenderingSystem*)customArg;

	GeometryHelper helper(rs);
	helper.destroyGeometry(geom);
}

void* NAV::core()
{
	return ready ? geom : 0;
}

#include <PGN/Assets/PNM.h>
#include <PGN/FileStream/FileStream.h>
#include <PGN/Math/Math.h>
#include <PGN/RenderingSystem/Buffer.h>
#include <PGN/RenderingSystem/RenderingSystem.h>
#include <PGN/RenderingSystem/VertexInputLayout.h>
#include <PGN/Utilities/CircularBuffer.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplate.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplateFactory.h>
#include "../Geometry.h"
#include "../GeometryHelper.h"
#include "PNM.h"

PNM::PNM(pgn::AssetFactory* factory, Geometry* geom, pgn::SkeletonTemplateFactory* skeletonFactory)
	: Asset(factory)
{
	this->geom = geom;
	this->skeletonFactory = skeletonFactory;
	ready = false;
}

bool PNM::cook(void* rawData)
{
	pgn::PNMHeader* header = (pgn::PNMHeader*)rawData;
	geom->skeletonTemplate = header->numBones ? skeletonFactory->createSkeletonTemplate(header->numBones, (pgn::Float4x3*)((char*)header + header->offsetMatsChunkOffset), (pgn::Float4*)((char*)header + header->defaultRotChunkOffset), (pgn::Float3*)((char*)header + header->defaultPosChunkOffset), (unsigned char*)((char*)header + header->parentIndicesChunkOffset)) : 0;
	return true;
}

pgn::VertexAttribDesc attribDescTable[] =
{
	"position", pgn::FLOAT3, 0,
	"normal", pgn::FLOAT3, 0,
	"numAssignedBones", pgn::BYTE, 0,
	"boneIndices", pgn::UBYTE4, 0,
	"boneWeights", pgn::FLOAT4, 0,
	"materialIndex", pgn::UBYTE, 0,
	"texCoord0", pgn::FLOAT2, 0,
	"texCoord1", pgn::FLOAT2, 0,
};

bool PNM::submit(void* rawData, void* customArg)
{
	pgn::RenderingSystem* rs = (pgn::RenderingSystem*)customArg;
	pgn::PNMHeader* header = (pgn::PNMHeader*)rawData;

	pgn::VertexAttribDesc attribDescs[pgn::PNMHeader::NUM_ATTRIBS];
	void* attribs[pgn::PNMHeader::NUM_ATTRIBS];
	int numAttribs = 0;

	for(int i = 0; i < pgn::PNMHeader::NUM_ATTRIBS; i++)
	{
		if(header->attribChunkOffsetTable[i] >= 0)
		{
			attribDescs[numAttribs] = attribDescTable[i];
			attribs[numAttribs] = (char*)header + header->attribChunkOffsetTable[i];
			numAttribs++;
		}
	}

	GeometryHelper helper(rs);
	helper.createGeometry(geom, numAttribs, attribDescs, header->numVerts, header->numSubsets, header->numIndices, attribs, (char*)header + header->indicesChunkOffset);
	geom->primType = pgn::PrimType::TRIANGLE_LIST;

	if (header->materialChunkSize)
	{
		pgn::BufferDesc brdfCoeffBufDesc;
		brdfCoeffBufDesc.bindingType = pgn::CONSTANT_BUFFER;
		brdfCoeffBufDesc.usage = pgn::STATIC_DRAW;
		brdfCoeffBufDesc.size = header->materialChunkSize;

		pgn::Buffer* buf = rs->createBuffer(&brdfCoeffBufDesc);
		buf->update(0, brdfCoeffBufDesc.size, (char*)header + header->materialChunkOffset);

		geom->brdfCoeffBuf.buf = buf;
		geom->brdfCoeffBuf.offset = 0;
		geom->brdfCoeffBuf.size = brdfCoeffBufDesc.size;
	}

	rs->flush();
	ready = true;

	return true;
}

void PNM::unload(void* customArg)
{
	if (!ready)
		return;

	if (geom->skeletonTemplate) geom->skeletonTemplate->destroy();

	pgn::RenderingSystem* rs = (pgn::RenderingSystem*)customArg;

	GeometryHelper helper(rs);
	helper.destroyGeometry(geom);

	geom->brdfCoeffBuf.buf->destroy();
}

void* PNM::core()
{
	return ready ? geom : 0;
}

#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#include "Graphics.h"
#include "Model.h"
#include "Renderer/Geometry.h"

enum
{
	  DIFFUSE_MAP
	, LIGHTMAP
	, NORMAL_MAP
};

Model::Model(Graphics* graphics, TextureSetAllocator& texSetAllocator)
	: textureInfo(texSetAllocator)
{
	this->graphics = graphics;
	geomHandle = 0;
	textureInfo.texBindingPoints[DIFFUSE_MAP] = &graphics->renderer.texBindingPointMap["diffuseMap"];
	_complete = false;
	submittingStamp = 0;
}

void Model::releaseGeom()
{
	graphics->renderer.geomMgr->releaseResource(geomHandle);
}

void Model::init()
{
	pgn::ResourceManager* texMgr = graphics->renderer.texMgr;

	for (auto& texSet : textureInfo.textureSets)
		for (auto tex : texSet.textures)
			if (tex) texMgr->releaseResource(tex);

	textureInfo.textureSets.clear();

	if (geomHandle)
		releaseGeom();
}

void Model::dispose()
{
	init();
}

pgn::Model* Graphics::createModel()
{
	return new(modelPool->alloc()) Model(this, texSetAllocator);
}

void Model::_free()
{
	graphics->pendingModelRemovals.push_back(this);
}

void Model::setMesh(const char fileName[])
{
	init();
	geomHandle = fileName ? graphics->renderer.geomMgr->getResource(fileName) : 0;
	_complete = false;
}

void Model::setTexture(int subset, int index, const char fileName[])
{
	while ((int)textureInfo.textureSets.size() <= subset)
		textureInfo.textureSets.emplace_back();

	auto it = textureInfo.textureSets.begin();
	for (int i = 0; i < subset; i++) it++;

	pgn::ResourceHandle** textures = it->textures;

	if (textures[index])
		graphics->renderer.texMgr->releaseResource(textures[index]);

	textures[index] = fileName ? graphics->renderer.texMgr->getResource(fileName) : 0;

	_complete = false;
}

void Model::setDiffuseMap(int subset, const char fileName[])
{
	setTexture(subset, DIFFUSE_MAP, fileName);
}

bool Model::complete()
{
	if (!_complete)
	{
		Geometry* geom = (Geometry*)geomHandle->core();
		if (geom)
		{
			for (auto& texSet : textureInfo.textureSets)
				for (auto tex : texSet.textures)
					if (tex && !tex->core())
						goto ret;

			_complete = true;
		}
	}

ret:
	return _complete;
}

int Model::getNumSubsets()
{
	return (int)((Geometry*)geomHandle->core())->subsets.size();
}

pgn::SkeletonTemplate* Model::getSkeletonTemplate()
{
	return ((Geometry*)geomHandle->core())->skeletonTemplate;
}

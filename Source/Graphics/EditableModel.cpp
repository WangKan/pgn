#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#include "EditableModel.h"
#include "Graphics.h"
#include "Renderer/EditableGeometry.h"

EditableModel::EditableModel(Graphics* graphics, TextureSetAllocator& texSetAllocator)
	: Model(graphics, texSetAllocator)
{
}

void EditableModel::releaseGeom()
{
	graphics->renderer.editableGeomMgr->releaseResource(geomHandle);
}

pgn::EditableModel* Graphics::createEditableModel()
{
	return new(modelPool->alloc()) EditableModel(this, texSetAllocator);
}

void EditableModel::setMesh(char fileName[])
{
	init();
	geomHandle = fileName ? graphics->renderer.editableGeomMgr->getResource(fileName) : 0;
	_complete = false;
}

void EditableModel::getAabb(int subset, pgn::Float3* min, pgn::Float3* max)
{
	EditableGeometry* geom = (EditableGeometry*)geomHandle->core();
	*min = geom->subsets[subset].aabb.min;
	*max = geom->subsets[subset].aabb.max;
}

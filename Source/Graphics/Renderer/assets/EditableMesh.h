#include "Mesh.h"

class EditableGeometry;

class EditableMesh : public Mesh
{
public:
	EditableMesh(pgn::AssetFactory* factory, EditableGeometry* geom, pgn::SkeletonTemplateFactory* skeletonFactory);
	virtual bool cook(void* rawData);
	virtual bool submit(void* rawData, void* customArg);
	virtual void unload(void* customArg);
	virtual void* core();
};

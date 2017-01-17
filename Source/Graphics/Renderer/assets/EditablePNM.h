#include "PNM.h"

class EditableGeometry;

class EditablePNM : public PNM
{
public:
	EditablePNM(pgn::AssetFactory* factory, EditableGeometry* geom, pgn::SkeletonTemplateFactory* skeletonFactory);
	virtual bool cook(void* rawData);
	virtual bool submit(void* rawData, void* customArg);
	virtual void unload(void* customArg);
	virtual void* core();
};

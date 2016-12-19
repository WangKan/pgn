#include <PGN/Graphics/Model.h>
#include "Renderer/TextureInfo.h"

class Graphics;

enum class GeomType
{
	Geometry,
	EditableGeometry
};

class Model : public pgn::Model
{
public:
	Graphics* graphics;
	pgn::ResourceHandle* geomHandle;
	GeomType geomType;
	TextureInfo textureInfo;
	bool _complete;
	long long submittingStamp;

	Model(Graphics* graphics, TextureSetAllocator& texSetAllocator);
	void init();
	~Model();
	virtual void dispose();
	virtual void _free();
	virtual void setMesh(char fileName[]);
	void setTexture(int subset, int index, char fileName[]);
	virtual void setDiffuseMap(int subset, char fileName[]);
	virtual bool complete();
	virtual int getNumSubsets();
	virtual pgn::SkeletonTemplate* getSkeletonTemplate();

	virtual void setEditableMesh(char fileName[]);
	virtual void getAabb(int subset, pgn::Float3* min, pgn::Float3* max);
};

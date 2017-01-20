#include <PGN/Graphics/NavModel.h>

namespace pgn {

class ResourceHandle;

}

class Graphics;

class NavModel : public virtual pgn::NavModel
{
public:
	Graphics* graphics;
	pgn::ResourceHandle* geomHandle;
	bool _complete;
	long long submittingStamp;

	NavModel(Graphics* graphics);
	void init();
	virtual void dispose();
	virtual void _free();
	virtual void setMesh(char fileName[]);
	virtual bool complete();
	virtual void getAabb(pgn::Float3* min, pgn::Float3* max);
};

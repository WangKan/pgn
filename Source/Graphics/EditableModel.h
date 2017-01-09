#include <PGN/Graphics/EditableModel.h>
#include "Model.h"

class EditableModel : public virtual pgn::EditableModel, public virtual Model
{
public:
	EditableModel(Graphics* graphics, TextureSetAllocator& texSetAllocator);
	void releaseGeom();
	virtual void _free();
	virtual void setMesh(char fileName[]);
	virtual void getAabb(int subset, pgn::Float3* min, pgn::Float3* max);
};

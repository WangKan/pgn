#pragma once
#include "Model.h"
namespace pgn {

struct Float3;

class EditableModel : public virtual Model
{
public:
	virtual void getAabb(int subset, Float3* min, Float3* max) = 0;
};

}

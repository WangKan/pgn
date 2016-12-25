#pragma once
#include "Model.h"
namespace pgn {

class EditableModel : public virtual Model
{
public:
	virtual void getAabb(int subset, Float3* min, Float3* max) = 0;
};

}

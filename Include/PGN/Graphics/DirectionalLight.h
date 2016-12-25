#pragma once
#include "../Common/Interface.h"
namespace pgn {

class DirectionalLight : public Interface
{
public:
	virtual void setIntensity(float r, float g, float b, float spec) = 0;
};

}

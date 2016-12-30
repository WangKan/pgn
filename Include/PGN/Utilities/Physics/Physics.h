#pragma once
#include "../../Common/Interface.h"
namespace pgn {

class Physics : public Interface
{
public:
	virtual void set(char fileName[]) = 0;
	virtual bool complete() = 0;
};

}

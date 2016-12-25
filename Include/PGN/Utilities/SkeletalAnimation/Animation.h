#pragma once
#include "../../Common/Interface.h"
namespace pgn {

class Animation : public Interface
{
public:
	virtual void set(char fileName[]) = 0;
	virtual bool complete() = 0;
};

}

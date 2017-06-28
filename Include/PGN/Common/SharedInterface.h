#pragma once
#include "Interface.h"
namespace pgn {

class SharedInterface : public Interface
{
public:
	virtual void retain() = 0;
	virtual void release() = 0;
};

}

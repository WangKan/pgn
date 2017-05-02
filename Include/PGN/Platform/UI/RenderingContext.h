#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
#include "Display.h"

namespace pgn {

class RenderingContext : public Interface
{
public:
	static PGN_DLL_INTERFACE RenderingContext* create(Display displayPrototype, int numThreads);
	virtual void beginDraw(Display display, int thread) = 0;
	virtual void endDraw() = 0;
	virtual void beginOnscreenPass() = 0;
};

}

#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class PipelineStage
{
public:
	virtual void onBegin() = 0;
	virtual void onEnd() = 0;
	virtual void process(void* p) = 0;
};

class Pipeline : public Interface
{
public:
	static PGN_DLL_INTERFACE Pipeline* create(int itemSize, int maxNumInFlightProducts, int numStages, PipelineStage* stages[], bool autoGet);

	virtual bool put(void* item) = 0;
	virtual void* get() = 0;
	virtual void finish() = 0;
};

}

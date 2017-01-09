#pragma once
#include "../Common/Interface.h"
namespace pgn {

class SkeletonTemplate;

class Model : public Interface
{
public:
	virtual void setMesh(char fileName[]/*0���*/) = 0;
	virtual void setDiffuseMap(int subset, char fileName[]/*0���*/) = 0;
	virtual bool complete() = 0;
	virtual int getNumSubsets() = 0;
	virtual SkeletonTemplate* getSkeletonTemplate() = 0;
};

}

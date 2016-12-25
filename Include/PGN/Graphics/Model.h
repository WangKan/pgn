#pragma once
#include "../Common/Interface.h"
namespace pgn {

struct Float3;
class SkeletonTemplate;

class Model : public Interface
{
public:
	virtual void setMesh(char fileName[]/*0Çå³ý*/) = 0;
	virtual void setDiffuseMap(int subset, char fileName[]/*0Çå³ý*/) = 0;
	virtual bool complete() = 0;
	virtual int getNumSubsets() = 0;
	virtual SkeletonTemplate* getSkeletonTemplate() = 0;

	virtual void setEditableMesh(char fileName[]/*0Çå³ý*/) = 0;
	virtual void getAabb(int subset, Float3* min, Float3* max) = 0;
};

}

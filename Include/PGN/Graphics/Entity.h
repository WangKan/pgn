#pragma once
#include "../Common/Interface.h"
namespace pgn {

struct Float4x3;
class Model;
class Skeleton;

class Entity : public Interface
{
public:
	virtual void setModel(Model* model) = 0;
	virtual void setSkeleton(Skeleton* skel) = 0;
	virtual void setPose(Float4x3* boneMats) = 0;
};

}

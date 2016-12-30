#include <PGN/Utilities/Physics/Physics.h>

namespace pgn {
	class ResourceHandle;
	class ResourceManager;
	class Pool;
}

class Physics : public pgn::Physics
{
public:
	pgn::Pool* pool;
	pgn::ResourceManager* physicsMgr;
	pgn::ResourceHandle* physics;
	bool _complete;

	Physics(pgn::Pool* pool, pgn::ResourceManager* physicsMgr);
	void init();
	virtual void dispose();
	virtual void _free();
	virtual void set(char fileName[]);
	virtual bool complete();
};

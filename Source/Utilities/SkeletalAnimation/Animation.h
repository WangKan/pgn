#include <PGN/Utilities/SkeletalAnimation/Animation.h>

namespace pgn {
	class ResourceHandle;
	class ResourceManager;
	class Pool;
}

class Animation : public pgn::Animation
{
public:
	pgn::Pool* pool;
	pgn::ResourceManager* animMgr;
	pgn::ResourceHandle* anim;
	bool _complete;

	Animation(pgn::Pool* pool, pgn::ResourceManager* animMgr);
	void init();
	virtual void dispose();
	virtual void _free();
	virtual void set(const char fileName[]);
	virtual bool complete();
};

#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#undef PGN_STATIC_IMPORT
#include "Animation.h"

Animation::Animation(pgn::Pool* pool, pgn::ResourceManager* animMgr)
{
	this->pool = pool;
	this->animMgr = animMgr;
	anim = 0;
	_complete = false;
}

void Animation::init()
{
	if (anim) animMgr->releaseResource(anim);
}

void Animation::dispose()
{
	init();
}

void Animation::set(const char fileName[])
{
	init();
	anim = fileName ? animMgr->getResource(fileName) : 0;
	_complete = false;
}

bool Animation::complete()
{
	if (!_complete)
		_complete = anim->core() != 0;

	return _complete;
}

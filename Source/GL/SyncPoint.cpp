#include <cassert>
#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/GL.h>
#include <PGN/RenderingSystem/SyncPoint.h>
#include "RenderingSystem.h"

class SyncPoint : public pgn::SyncPoint
{
public:
	GLsync sync;

	SyncPoint();
	virtual void dispose();
	virtual void _free();
};

SyncPoint::SyncPoint()
{
	sync = 0;
}

void SyncPoint::dispose()
{
	if (sync) glDeleteSync(sync);
}

pgn::SyncPoint* RenderingSystem::createSyncPoint()
{
	return debug_new SyncPoint;
}

void SyncPoint::_free()
{
	delete this;
}

void RenderingSystem::addSyncPoint(SyncPoint* sync)
{
	assert(!sync->sync);
	sync->sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

bool RenderingSystem::checkSyncPoint(pgn::SyncPoint* _sync)
{
	SyncPoint* sync = (SyncPoint*)_sync;

	int status;
	glGetSynciv(sync->sync, GL_SYNC_STATUS, 1, NULL, &status);

	if (status == GL_UNSIGNALED)
		return false;

	glDeleteSync(sync->sync);
	sync->sync = 0;
	return true;
}

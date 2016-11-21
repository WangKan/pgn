#define PGN_DLL_EXPORT
#include <PGN/Platform/UI/GL.h>
#undef PGN_DLL_EXPORT

class WGL
{
public:
	pgn::GLInfo glInfo;
	int refCount;

	WGL();
	void addRef();
	void release();
};

extern WGL wgl;

#include "GLInfo.h"

pgn::GLInfo* pgn::getGLInfo()
{
	return &glInfo;
}

char* pgn::getGLSLVersionDecl()
{
	return "#version 300 es";
}

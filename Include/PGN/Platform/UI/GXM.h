#pragma once
#include "../../Common/DllInterface.h"
#include <gxm/context.h>
#include <kernel/sysmem/memblock.h>

extern PGN_DLL_INTERFACE SceGxmContext* gxmContext;

namespace pgn {

PGN_DLL_INTERFACE unsigned getFreeVideoMemSize();
PGN_DLL_INTERFACE void *graphicsAlloc(SceKernelMemBlockType type, unsigned size, unsigned alignment, unsigned attribs, SceUID *uid);
PGN_DLL_INTERFACE void graphicsFree(SceUID uid);
PGN_DLL_INTERFACE void *vertexUsseAlloc(unsigned size, SceUID *uid, unsigned *usseOffset);
PGN_DLL_INTERFACE void vertexUsseFree(SceUID uid);
PGN_DLL_INTERFACE void *fragmentUsseAlloc(unsigned size, SceUID *uid, unsigned *usseOffset);
PGN_DLL_INTERFACE void fragmentUsseFree(SceUID uid);

}

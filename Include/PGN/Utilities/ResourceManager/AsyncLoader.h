#pragma once
#include <PGN/Common/DllInterface.h>
#include <PGN/Platform/UI/Display.h>
namespace pgn {

class AssetLoader;
class RenderingContext;
class RenderingSystem;

PGN_DLL_INTERFACE AssetLoader* createAsyncLoader();
PGN_DLL_INTERFACE AssetLoader* createAsyncLoader(pgn::RenderingContext* rc, pgn::RenderingSystem* rs, pgn::Display display);
PGN_DLL_INTERFACE void destroyAsyncLoader(AssetLoader* loader);

PGN_DLL_INTERFACE AssetLoader* getDefaultAsyncLoader();
PGN_DLL_INTERFACE void releaseDefaultAsyncLoader();

}

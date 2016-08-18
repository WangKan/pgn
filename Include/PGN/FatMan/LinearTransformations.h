#pragma once
#include <PGN/Math/Math.h>
namespace pgn {

const int turnDiv = 512; // һȦ512��

PGN_DLL_INTERFACE void computeWorldMat(pgn::Float4x3* result, float x, float y, float z, int yaw);

}

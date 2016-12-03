#pragma once
#include "../Common/DllInterface.h"
namespace pgn {

struct Float4x3;

// 不需要太高精度的时候，角的度数可以用小范围的整数表示。turnDiv = 512表示一圈512度（0到511）。
const int turnDiv = 512;

PGN_DLL_INTERFACE void buildWorldMat(float x, float y, float z, int yaw, Float4x3* result);

}

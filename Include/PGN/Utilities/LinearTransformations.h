#pragma once
#include "../Common/DllInterface.h"
namespace pgn {

struct Float4x3;

// ����Ҫ̫�߾��ȵ�ʱ�򣬽ǵĶ���������С��Χ��������ʾ��turnDiv = 512��ʾһȦ512�ȣ�0��511����
const int turnDiv = 512;

PGN_DLL_INTERFACE void buildWorldMat(float x, float y, float z, int yaw, Float4x3* result);

}

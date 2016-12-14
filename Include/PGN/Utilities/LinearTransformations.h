#pragma once
#include "../Common/DllInterface.h"
namespace pgn {

struct Float3;
struct Float4x3;

// ����Ҫ̫�߾��ȵ�ʱ�򣬽ǵĶ���������С��Χ��������ʾ��turnDiv = 512��ʾһȦ512�ȣ�0��511����
const int turnDiv = 512;

PGN_DLL_INTERFACE void buildWorldMat(float x, float y, float z, int yaw, Float4x3* result);

PGN_DLL_INTERFACE void buildViewMat(Float3* _eye, Float3* _lookAt, Float3* _up, Float4x3* _result);
PGN_DLL_INTERFACE void buildViewMat(Float4x3* baseViewMat, Float3* pivotInWorldSpace, float yaw, float pitch, Float4x3* result);

PGN_DLL_INTERFACE void calculateInverseViewMat(Float4x3* viewMat, Float4x3* result);

}

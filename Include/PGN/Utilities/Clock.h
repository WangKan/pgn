#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class Clock : public Interface
{
public:
	// ����һ��ʱ��
	// ʱ�����ڳ�ʼ��ΪcycleNumer/cycleDenom��
	static PGN_DLL_INTERFACE Clock* create(long long cycleNumer = 1, long long cycleDenom = 1000);

	// �������״ε���������ȥ��ʱ�����ڸ���
	virtual long long getTickCount() = 0;

	// ����ʱ������Ϊnumer/denom��
	virtual void setCycle(long long numer, long long denom) = 0;

	// ���ʱ������
	virtual void getCycle(long long* numer, long long* denom) = 0;

	// ����ʱ��
	// �ٴε���getTickCount()���¿�ʼ����
	virtual void reset() = 0;
};

}

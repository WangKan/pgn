#pragma once
#include "../Common/Interface.h"
namespace pgn {

class Program : public Interface
{
public:
	virtual int getNumTextureVariables() = 0;
	virtual const char* getTextureVariableName(int i) = 0; // ��ͬ���������Ƿ�����ͬ��ָ�룬��ʹ���ڲ�ͬ��Program��
};

}

#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class Text : public Interface
{
public:
	static PGN_DLL_INTERFACE Text* create(int sizeBuf);

	virtual void setText(char text[]) = 0;
	virtual void setText(const char text[]) = 0;
	virtual char* getText() = 0;
	virtual void replace(char pattern[], char replacement[], bool replaceAll = false) = 0;	// С���������
	virtual bool extract(char pattern[], char* s, int* i) = 0;
};

}

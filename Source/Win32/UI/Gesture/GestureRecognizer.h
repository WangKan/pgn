#include <Windows.h>

class GestureRecognizer
{
public:
	virtual void processInput(UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

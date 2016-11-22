#include "../../../Common/DllInterface.h"
#include "GestureRecognizer.h"
namespace pgn {

class DraggingGestureRecognizer : public GestureRecognizer
{
public:
	static PGN_DLL_INTERFACE DraggingGestureRecognizer* create();

	virtual void getStart(int* x, int* y) = 0;
	virtual void getPosition(int* x, int* y) = 0;
	virtual void getVelocity(int* x, int* y) = 0;
};

}

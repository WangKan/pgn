#pragma once
#include "../../../Common/Interface.h"
namespace pgn {

enum GestureRecognizerState
{
	inputNotRecognized,
	gestureRecognized,
	gestureEnded,
	gestureCancelled
};

class GestureRecognizer : public Interface
{
public:
	virtual void update() = 0;
	virtual GestureRecognizerState getState() = 0;
};

}

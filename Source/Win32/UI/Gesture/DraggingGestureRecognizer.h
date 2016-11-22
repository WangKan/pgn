#include <PGN/Platform/UI/Gesture/DraggingGestureRecognizer.h>
#include <PGN/Utilities/Queue.h>
#include "GestureRecognizer.h"

enum InternalState
{
	inputNotRecognized,
	posible,
	gestureRecognized
};

struct Report
{
	pgn::GestureRecognizerState state;
	int start[2];
	int pos[2];
	int velocity[2];
};

class DraggingGestureRecognizer : public pgn::DraggingGestureRecognizer, public GestureRecognizer
{
	InternalState internalState;
	int start[2];

	pgn::Queue<Report, 4> reportQueue;
	Report curReport;

public:
	DraggingGestureRecognizer();
	virtual void dispose();
	virtual void _free();

	virtual void update();
	virtual pgn::GestureRecognizerState getState();

	virtual void getStart(int* x, int* y);
	virtual void getPosition(int* x, int* y);
	virtual void getVelocity(int* x, int* y);

	virtual void processInput(UINT message, WPARAM wParam, LPARAM lParam);
};

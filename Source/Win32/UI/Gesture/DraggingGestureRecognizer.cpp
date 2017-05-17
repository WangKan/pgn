#define PGN_DLL_EXPORT
#include "DraggingGestureRecognizer.h"
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <Windowsx.h>

DraggingGestureRecognizer::DraggingGestureRecognizer()
	: reportQueue(4)
{
	internalState = inputNotRecognized;

	//EnableMouseInPointer(TRUE);
}

void DraggingGestureRecognizer::dispose()
{

}

pgn::DraggingGestureRecognizer* pgn::DraggingGestureRecognizer::create()
{
	return debug_new ::DraggingGestureRecognizer;
}

void DraggingGestureRecognizer::_free()
{
	delete this;
}

void DraggingGestureRecognizer::update()
{
	if (!reportQueue.empty())
	{
		curReport = reportQueue.front();
		reportQueue.pop();
	}
	else
	{
		if (internalState == inputNotRecognized)
			curReport.state = pgn::inputNotRecognized;
	}
}

pgn::GestureRecognizerState DraggingGestureRecognizer::getState()
{
	return curReport.state;
}

void DraggingGestureRecognizer::getStart(int* x, int* y)
{
	*x = curReport.start[0];
	*y = curReport.start[1];
}

void DraggingGestureRecognizer::getPosition(int* x, int* y)
{
	*x = curReport.pos[0];
	*y = curReport.pos[1];
}

void DraggingGestureRecognizer::getVelocity(int* x, int* y)
{
	*x = curReport.velocity[0];
	*y = curReport.velocity[1];
}

void DraggingGestureRecognizer::processInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (internalState)
	{
	case inputNotRecognized:

		if (message == WM_LBUTTONDOWN)
		{
			internalState = posible;
			start[0] = GET_X_LPARAM(lParam);
			start[1] = GET_Y_LPARAM(lParam);
		}

		break;

	case posible:

		if (message == WM_MOUSEMOVE)
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);

			int dx = x - start[0];
			int dy = y - start[1];

			if (dx*dx + dy*dy >= 5*5)
			{
				internalState = gestureRecognized;

				Report report;
				report.state = pgn::gestureRecognized;
				report.start[0] = start[0];
				report.start[1] = start[1];
				report.pos[0] = GET_X_LPARAM(lParam);
				report.pos[1] = GET_Y_LPARAM(lParam);
				report.velocity[0] = 0;
				report.velocity[1] = 0;

				if (!reportQueue.full())
					reportQueue.push(report);
			}
		}
		else if (message == WM_LBUTTONUP)
		{
			internalState = inputNotRecognized;
		}

		break;

	case gestureRecognized:

		Report report;

		if (message == WM_MOUSEMOVE)
		{
			report.state = pgn::gestureRecognized;
		}
		else if (message == WM_LBUTTONUP)
		{
			report.state = pgn::gestureEnded;
			internalState = inputNotRecognized;
		}

		report.start[0] = start[0];
		report.start[1] = start[1];
		report.pos[0] = GET_X_LPARAM(lParam);
		report.pos[1] = GET_Y_LPARAM(lParam);
		report.velocity[0] = 0;
		report.velocity[1] = 0;

		if (!reportQueue.empty())
		{
			Report& back = reportQueue.back();

			if (back.state == pgn::gestureRecognized)
				back = report;
			else if (!reportQueue.full())
				reportQueue.push(report);
		}
		else
		{
			reportQueue.push(report);
		}

		break;

	default:
		break;
	}
}

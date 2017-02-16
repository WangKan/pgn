#include <math.h>
#include <PGN/Common/MemAlign.h>
#include "sinCosTable.h"

_aligned(4096, pgn::Float2 sinCosTable[pgn::turnDiv]);

class SinCosTableBuilder
{
public:
	SinCosTableBuilder()
	{
		double pi = atan(1.0) * 4.0;
		double a = 2.0 * pi / pgn::turnDiv;

		for (int i = 0; i < pgn::turnDiv; i++)
		{
			double x = i * a;
			sinCosTable[i].x = (float)sin(x);
			sinCosTable[i].y = (float)cos(x);
		}
	}
};

static SinCosTableBuilder sinCosTableBuilder;

#include "CommonVSOutput.h"

struct Movable2D
{
	mat2x3 transf;
	vec2 scale;
};

uniform Movable2DBlock@1			// enum BindingPoint
{
	Movable2D instances[256];
};

in vec2 position2D;
in vec2 texCoord0;

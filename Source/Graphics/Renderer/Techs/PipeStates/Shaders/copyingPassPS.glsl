#include "CommonPS.h"

uniform sampler2D rawImage#0;

void main()
{
	highp vec2 tc = vec2(tc0.x, 1.0 - tc0.y);
	highp vec2 xy = tc * 2.0 - 1.0;

	lowp vec4 c = texture(rawImage, tc);

	color[0] = c;
}

#include "CommonPS.h"

uniform sampler2D diffuseMap#0;

void main()
{
	color[0] = texture(diffuseMap, tc0);
}

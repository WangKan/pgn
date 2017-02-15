#include "CommonVS2D.h"

void main()
{
	vec2 pos;
	pos = position2D * instances[0].scale;
	pos = vec3(pos, 1.0) * instances[0].transf;
	gl_Position = vec4(pos, -1.0, 1.0);
	tc0 = texCoord0;
}

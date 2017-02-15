#include "MovableBlock.h"
#include "ZOnlyPassEnvCBlock.h"

in vec3 position;

void main()
{
	vec3 pos = position * instances[gl_InstanceID].scale.xyx;

	mat3x4 world = instances[gl_InstanceID].world;

	vec3 wpos = vec4(pos, 1.0) * world;

	gl_Position = vec4(wpos, 1.0) * viewProj;
}

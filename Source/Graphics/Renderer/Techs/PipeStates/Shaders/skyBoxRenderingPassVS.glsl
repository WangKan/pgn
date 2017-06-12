#include "CommonVS3D.h"

uniform EnvCBlock@0 // enum BindingPoint
{
	highp mat4 viewProj;
};

void main()
{
	mat3x4 world = instances[gl_InstanceID].world;

	vec3 mpos = position * instances[gl_InstanceID].scale.xyx;
	vec3 wpos = vec4(mpos, 1.0) * world;
	vec4 pos = vec4(wpos, 1.0) * viewProj;

	pos.z = pos.w;

	gl_Position = pos;
	tc0 = texCoord0;
}

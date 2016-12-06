#include "CommonVS3DInput.h"
#include "PointLightVolumnInstanceCBlock.h"

uniform EnvCBlock@0 // enum BindingPoint
{
	mat4 viewProj;
};

void main()
{
	vec4 pos_scale = instances[gl_InstanceID].pos_scale;
	vec3 pos = position * pos_scale.w + pos_scale.xyz;

	gl_Position = vec4(pos, 1.0) * viewProj;
}

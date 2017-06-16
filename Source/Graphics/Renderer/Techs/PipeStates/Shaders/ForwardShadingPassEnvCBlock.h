#include "DirectionalLight.h"
#include "PointLight.h"

uniform EnvCBlock@0 // enum BindingPoint
{
	highp mat4 viewProj;
	highp vec3 camPos;
	PointLight wPointLights[256];
	DirectionalLight wDirLights[8];
};

#include "CommonPS.h"

flat in lowp float Ks;
flat in lowp float shininess;

uniform sampler2D diffuseMap#0;

void main()
{
	lowp vec4 diffuse_Ks;
	diffuse_Ks.rgb = texture(diffuseMap, tc0).rgb;
	diffuse_Ks.a = Ks;

	lowp vec4 normal_shininess;
	normal_shininess.xyz = n;
	normal_shininess.w = shininess;

	color[0] = diffuse_Ks;
	color[1] = normal_shininess;
}

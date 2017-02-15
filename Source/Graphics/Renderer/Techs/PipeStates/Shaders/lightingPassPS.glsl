#include "CommonPS.h"
#include "DirectionalLight.h"
#include "PointLight.h"

uniform EnvCBlock@0 // enum BindingPoint
{
	highp mat4 invProj;
	PointLight vPointLights[256];
	DirectionalLight vDirLights[8];
};

uniform sampler2D gbuf0#0;
uniform sampler2D gbuf1#1;
uniform lowp usampler2D lightIndexMap#2;
uniform highp sampler2D depthStencilMap#3;

void main()
{
	highp vec2 tc = vec2(tc0.x, 1.0 - tc0.y);
	highp vec2 xy = tc * 2.0 - 1.0;

	lowp vec4 diffuse_Ks = texture(gbuf0, tc);
	lowp vec4 normal_shininess = texture(gbuf1, tc);
	lowp uvec4 lightIndices = texture(lightIndexMap, tc);
	highp float depth = texture(depthStencilMap, tc).r * 2.0 - 1.0;

	highp vec3 pos;
	pos.z = 1.0 / (depth * invProj[3][2] + invProj[3][3]);
	pos.x = xy.x * invProj[0][0] * pos.z;
	pos.y = xy.y * invProj[1][1] * pos.z;

	lowp vec3 N = normal_shininess.xyz * 2.0 - 1.0;
	lowp vec4 I = vec4(0.0, 0.0, 0.0, 1.0);

	for (int i = 0; i < 4; i++)
	{
		lowp uint lightIndex = lightIndices[i];

		if (lightIndex == 255u)
			continue;

		PointLight light = vPointLights[lightIndex];

		highp vec3 lightVec = light.pos_att.xyz - pos;
		highp float dist = length(lightVec);
		highp float invDist = 1.0 / dist;
		lowp vec3 L = lightVec * invDist;
		lowp float NdotL = max(0.0, dot(N,L));
		lowp vec3 R = 2.0 * NdotL * N - L;
		lowp vec3 V = normalize(-pos);
		lowp float VdotR = max(0.0, dot(V,R));
		mediump float shininess = normal_shininess.w * 100.0;
		lowp float att = 1.0 / (light.pos_att.w * dist*dist*dist*dist + 1.0);

		I.rgb += (diffuse_Ks.rgb * NdotL + diffuse_Ks.a * pow(VdotR, shininess) * light.intensity_spec.a) * att * light.intensity_spec.rgb;
	}

	for (int i = 0; i < 4; i++)
	{
		DirectionalLight light = vDirLights[i];

		if (light.dir_enabled.w == 0.0) continue;

		lowp vec3 L = -light.dir_enabled.xyz;
		lowp float NdotL = max(0.0, dot(N,L));
		lowp vec3 R = 2.0 * NdotL * N - L;
		lowp vec3 V = normalize(-pos);
		lowp float VdotR = max(0.0, dot(V,R));
		mediump float shininess = normal_shininess.w * 100.0;
		I.rgb += (diffuse_Ks.rgb * NdotL + diffuse_Ks.a * pow(VdotR, shininess) * light.intensity_spec.a) * light.intensity_spec.rgb;
	}

	color[0] = I;
}

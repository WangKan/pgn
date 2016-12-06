#include "CommonPS.h"
#include "ForwardShadingPassEnvCBlock.h"

in highp vec3 wpos;
flat in lowp float Ks;
flat in lowp float shininess;

uniform sampler2D diffuseMap#0;
uniform lowp usampler2D lightIndexMap#1;

void main()
{
	lowp vec3 diffuse = texture(diffuseMap, tc0).rgb;
	lowp uvec4 lightIndices = texelFetch(lightIndexMap, ivec2(gl_FragCoord.xy), 0);

	lowp vec4 I = vec4(0.0, 0.0, 0.0, 1.0);

	for (int i = 0; i < 4; i++)
	{
		lowp uint lightIndex = lightIndices[i];

		if (lightIndex == 255u)
			continue;

		PointLight light = wPointLights[lightIndex];

		highp vec3 lightVec = light.pos_att.xyz - wpos;
		highp float dist = length(lightVec);
		highp float invDist = 1.0 / dist;
		lowp vec3 L = lightVec * invDist;
		lowp float NdotL = max(0.0, dot(n,L));
		lowp vec3 R = 2.0 * NdotL * n - L;
		lowp vec3 V = normalize(camPos - wpos);
		lowp float VdotR = max(0.0, dot(V,R));
		lowp float att = 1.0 / (light.pos_att.w * dist*dist*dist*dist + 1.0);

		I.rgb += (diffuse * NdotL + Ks * pow(VdotR, shininess) * light.intensity_spec.a) * att * light.intensity_spec.rgb;
	}

	for (int i = 0; i < 4; i++)
	{
		DirectionalLight light = wDirLights[i];

		if (light.dir_enabled.w == 0.0) continue;

		lowp vec3 L = -light.dir_enabled.xyz;
		lowp float NdotL = max(0.0, dot(n,L));
		lowp vec3 R = 2.0 * NdotL * n - L;
		lowp vec3 V = normalize(-wpos);
		lowp float VdotR = max(0.0, dot(V,R));
		I.rgb += (diffuse * NdotL + Ks * pow(VdotR, shininess) * light.intensity_spec.a) * light.intensity_spec.rgb;
	}

	color[0] = I;
}

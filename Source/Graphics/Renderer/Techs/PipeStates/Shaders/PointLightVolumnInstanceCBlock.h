
struct PointLightVolumnInstance
{
	highp vec4 pos_scale;
	highp int lightIndex;
};

uniform PointLightVolumnInstanceCBlock@1	// enum BindingPoint
{
	PointLightVolumnInstance instances[256];
};

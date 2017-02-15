
struct Material
{
	highp float Ks;
	highp float shininess;
};

uniform MaterialBlock@3				// enum BindingPoint
{
	Material materials[256];
};


struct Movable
{
	mat3x4 world;
	vec2 scale;
};

uniform MovableBlock@1				// enum BindingPoint
{
	Movable instances[256];
};

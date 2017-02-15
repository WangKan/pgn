
flat in uint lightIndex;
out uvec4 color[4];

void main()
{
	color[0] = uvec4(lightIndex);
}

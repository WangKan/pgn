
public ref class DirtyFlags
{
public:
	bool mesh;
	bool diffuseMap;
	bool anims;

	bool curAnim;

	DirtyFlags()
	{
		mesh = true;
		diffuseMap = true;
		anims = true;

		curAnim = true;
	}

	void clear()
	{
		mesh = false;
		diffuseMap = false;
		anims = false;

		curAnim = false;
	}
};


public ref class DirtyFlags
{
public:
	bool mesh;
	bool diffuseMap;
	bool anims;
	bool navMesh;

	bool curAnim;

	DirtyFlags()
	{
		mesh = true;
		diffuseMap = true;
		anims = true;
		navMesh = true;

		curAnim = true;
	}

	void clear()
	{
		mesh = false;
		diffuseMap = false;
		anims = false;
		navMesh = false;

		curAnim = false;
	}
};

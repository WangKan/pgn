#pragma once

public ref class DirtyFlags
{
public:
	bool clearView;
	bool name;
	bool file;
	bool worldMat;
	bool del;

	DirtyFlags()
	{
		clearView = false;
		name = true;
		file = true;
		worldMat = true;
		del = false;
	}

	void clear()
	{
		clearView = false;
		name = false;
		file = false;
		worldMat = false;
		del = false;
	}
};


#pragma once

ref class SceneEntityProperties;
public ref class Options
{
	System::String^ _tempDir;
	bool _verticalFovFixed;
	float _fov;

public:
	Options()
	{
#undef GetTempPath
		_tempDir = System::IO::Path::GetTempPath() + "PGN\\";
		System::IO::Directory::CreateDirectory(_tempDir);
	}

	property System::String^ tempDir
	{
		System::String^ get()
		{
			return _tempDir;
		}

		void set(System::String^ tempDir)
		{
			_tempDir = tempDir;
			System::IO::Directory::CreateDirectory(_tempDir);
		}
	}

	property float verticalFov
	{
		void set(float fov)
		{
			_verticalFovFixed = true;
			_fov = fov;
		}
	}

	property float horizontalFov
	{
		void set(float fov)
		{
			_verticalFovFixed = false;
			_fov = fov;
		}
	}

	property bool verticalFovFixed
	{
		bool get()
		{
			return _verticalFovFixed;
		}
	}

	property float fov
	{
		float get()
		{
			return _fov;
		}
	}
};


#include "DirtyFlags.h"

ref class Anim;

public ref class Options
{
	DirtyFlags^ dirtyFlags;
	System::String^ _tempDir;
	bool _verticalFovFixed;
	float _fov;

	void Options::OnAddingNew(System::Object ^sender, System::ComponentModel::AddingNewEventArgs ^e)
	{
		dirtyFlags->curAnim = true;
	}

	void Options::OnListChanged(System::Object ^sender, System::ComponentModel::ListChangedEventArgs ^e)
	{
		dirtyFlags->curAnim = true;
	}

public:
	Options(DirtyFlags^ dirtyFlags)
	{
		this->dirtyFlags = dirtyFlags;

#undef GetTempPath
		_tempDir = System::IO::Path::GetTempPath() + "PGN\\";
		System::IO::Directory::CreateDirectory(_tempDir);

		curAnims = gcnew System::ComponentModel::BindingList<Anim^>;
		curAnims->AddingNew += gcnew System::ComponentModel::AddingNewEventHandler(this, &Options::OnAddingNew);
		curAnims->ListChanged += gcnew System::ComponentModel::ListChangedEventHandler(this, &Options::OnListChanged);
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

	property System::ComponentModel::BindingList<Anim^>^ curAnims;

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

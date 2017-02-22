#pragma once

#include "DirtyFlags.h"
#include "Utilities.h"

using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Windows::Forms::Design;

public ref class SceneEntityProperties
{
	String^ _entityFile;
	String^ _entityName;
	float _position_x, _position_y, _position_z;
	float _scale_x, _scale_y, _scale_z;
	DirtyFlags^ _dirtyFlags;
	EntityProperties^ _props;

public:
	SceneEntityProperties()
	{
		_position_x = _position_y = _position_z = 0.0f;
		_scale_x = _scale_y = _scale_z = 1.0f;
		_dirtyFlags = nullptr;
		_props = nullptr;
	}

	SceneEntityProperties(EntityProperties^ props) : SceneEntityProperties()
	{
		_dirtyFlags = gcnew DirtyFlags;
		_props = props;
	}

	[CategoryAttribute("实体"), DisplayNameAttribute("文件")]
	[EditorAttribute(FileNameEditor::typeid, UITypeEditor::typeid)]
	property String^ entityFile
	{
		String^ get() { return _entityFile; }
		void set(String^ file) { _entityFile = file; }
	}

	[CategoryAttribute("实体"), DisplayNameAttribute("名称")]
	property String^ entityName
	{
		String^ get() { return _entityName; }
		void set(String^ file) { _entityName = file; }
	}

	[CategoryAttribute("坐标"), DisplayNameAttribute("x")]
	property float position_x
	{
		float get() { return _position_x; }
		void set(float p)
		{
			_position_x = p;
			if (_dirtyFlags)
				_dirtyFlags->worldMat = true;
		}
	}

	[CategoryAttribute("坐标"), DisplayNameAttribute("y")]
	property float position_y
	{
		float get() { return _position_y; }
		void set(float p)
		{
			_position_y = p;
			if (_dirtyFlags)
				_dirtyFlags->worldMat = true;
		}
	}

	[CategoryAttribute("坐标"), DisplayNameAttribute("z")]
	property float position_z
	{
		float get() { return _position_z; }
		void set(float p)
		{
			_position_z = p;
			if (_dirtyFlags)
				_dirtyFlags->worldMat = true;
		}
	}

	[CategoryAttribute("缩放"), DisplayNameAttribute("x")]
	property float scale_x
	{
		float get() { return _scale_x; }
		void set(float s)
		{
			_scale_x = s;
			if (_dirtyFlags)
				_dirtyFlags->worldMat = true;
		}
	}

	[CategoryAttribute("缩放"), DisplayNameAttribute("y")]
	property float scale_y
	{
		float get() { return _scale_y; }
		void set(float s)
		{
			_scale_y = s;
			if (_dirtyFlags)
				_dirtyFlags->worldMat = true;
		}
	}
	
	[CategoryAttribute("缩放"), DisplayNameAttribute("z")]
	property float scale_z
	{
		float get() { return _scale_z; }
		void set(float s)
		{
			_scale_z = s;
			if (_dirtyFlags)
				_dirtyFlags->worldMat = true;
		}
	}

	property DirtyFlags^ dirtyFlags
	{
		DirtyFlags^ get() { return _dirtyFlags;  }
	}

	property EntityProperties^ props
	{
		EntityProperties^ get() { return _props; }
	}
};


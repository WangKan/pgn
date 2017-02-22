#pragma once

#include "Options.h"
#include "Utilities.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Windows::Forms::Design;

public ref class Anim
{
	String^ _fileName;

public:
	Anim()
	{
		_fileName = "";
	}

	Anim(String^ fileName)
	{
		_fileName = fileName;
	}

	property String^ fileName
	{
		String^ get()
		{
			return _fileName;
		}

		void set(String^ fileName)
		{
			_fileName = fileName;
		}
	}

	property String^ displayName
	{
		String^ get()
		{
			return Utilities::getFileName(_fileName, ".PNA");
		}
	}
};

public ref class EntityProperties
{
	String^ _mesh;
	String^ _diffuseMap;
	String^ _navMesh;
	BindingList<Anim^>^ _anims;

public:
	EntityProperties()
	{
		_mesh = "";
		_diffuseMap = "";
		_navMesh = "";
		_anims = gcnew BindingList<Anim^>;
	}

	property String^ mesh
	{
		String^ get()
		{
			return _mesh;
		}
		void set(String^ mesh)
		{
			_mesh = mesh;
		}
	}

	property String^ diffuseMap
	{
		String^ get()
		{
			return _diffuseMap;
		}

		void set(String^ diffuseMap)
		{
			_diffuseMap = diffuseMap;
		}
	}

	property BindingList<Anim^>^ anims
	{
		BindingList<Anim^>^ get()
		{
			return _anims;
		}
	}

	property String^ navMesh
	{
		String^ get()
		{
			return _navMesh;
		}

		void set(String^ mesh)
		{
			_navMesh = mesh;
		}
	}
};


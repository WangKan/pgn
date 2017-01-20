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

	bool Equals(Object^ _v) override
	{
		if (_v->GetType() != Anim::typeid)
			return false;

		Anim^ v = (Anim^)_v;

		return v->_fileName == _fileName;
	}

	[CategoryAttribute("动画"), DisplayNameAttribute("动画文件")]
	[EditorAttribute(FileNameEditor::typeid, UITypeEditor::typeid)]
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
	Options^ options;
	DirtyFlags^ dirtyFlags;
	String^ _mesh;
	String^ _diffuseMap;
	BindingList<Anim^>^ _anims;
	String^ _navMesh;

	void OnAddingNew(System::Object ^sender, System::ComponentModel::AddingNewEventArgs ^e)
	{
		dirtyFlags->anims = true;
	}

	void OnListChanged(System::Object ^sender, System::ComponentModel::ListChangedEventArgs ^e)
	{
		dirtyFlags->anims = true;
	}

public:
	EntityProperties()
	{
		dirtyFlags = nullptr;
		_mesh = "";
		_diffuseMap = "";
		_anims = gcnew BindingList<Anim^>;
		_navMesh = "";
	}

	EntityProperties(Options^ options, DirtyFlags^ dirtyFlags)
		: EntityProperties()
	{
		this->options = options;
		this->dirtyFlags = dirtyFlags;
		_anims->AddingNew += gcnew System::ComponentModel::AddingNewEventHandler(this, &EntityProperties::OnAddingNew);
		_anims->ListChanged += gcnew System::ComponentModel::ListChangedEventHandler(this, &EntityProperties::OnListChanged);
	}

	void copy(EntityProperties^ props)
	{
		mesh = props->mesh;
		diffuseMap = props->diffuseMap;
		anims->Clear();

		for each(Anim^ anim in props->anims)
			anims->Add(anim);

		navMesh = props->navMesh;
	}

	EntityProperties^ getPortable()
	{
		EntityProperties^ portable = gcnew EntityProperties;

		portable->_mesh = Utilities::getFileName(_mesh, "");
		portable->_diffuseMap = Utilities::getFileName(_diffuseMap, ".TGA");

		for each(Anim^ anim in _anims)
			portable->_anims->Add(gcnew Anim(Utilities::getFileName(anim->fileName, "")));

		portable->_navMesh = Utilities::getFileName(_navMesh, "");

		return portable;
	}

	[CategoryAttribute("模型"), DisplayNameAttribute("模型文件")]
	[EditorAttribute(FileNameEditor::typeid, UITypeEditor::typeid)]
	property String^ mesh
	{
		String^ get()
		{
			return _mesh;
		}

		void set(String^ mesh)
		{
			_mesh = mesh;

			if (dirtyFlags)
				dirtyFlags->mesh = true;
		}
	}

	[CategoryAttribute("模型"), DisplayNameAttribute("纹理贴图")]
	[EditorAttribute(FileNameEditor::typeid, UITypeEditor::typeid)]
	property String^ diffuseMap
	{
		String^ get()
		{
			return _diffuseMap;
		}

		void set(String^ diffuseMap)
		{
			_diffuseMap = diffuseMap;

			if (dirtyFlags)
				dirtyFlags->diffuseMap = true;
		}
	}

	String^ getDiffuseMap_dxt_ktx()
	{
		return options->tempDir + Utilities::getFileName(_diffuseMap, ".TGA") + ".dxt.ktx";
	}

	String^ getDiffuseMap_dxt_pnt()
	{
		return options->tempDir + Utilities::getFileName(_diffuseMap, ".TGA") + ".dxt.PNT";
	}

	String^ getDiffuseMap_etc_ktx()
	{
		return options->tempDir + Utilities::getFileName(_diffuseMap, ".TGA") + ".etc.ktx";
	}

	String^ getDiffuseMap_etc_pnt()
	{
		return options->tempDir + Utilities::getFileName(_diffuseMap, ".TGA") + ".etc.PNT";
	}

	[CategoryAttribute("动画"), DisplayNameAttribute("动画")]
	property BindingList<Anim^>^ anims
	{
		BindingList<Anim^>^ get()
		{
			return _anims;
		}
	}

	[CategoryAttribute("导航模型"), DisplayNameAttribute("导航模型")]
	[EditorAttribute(FileNameEditor::typeid, UITypeEditor::typeid)]
	property String^ navMesh
	{
		String^ get()
		{
			return _navMesh;
		}

		void set(String^ mesh)
		{
			_navMesh = mesh;

			if (dirtyFlags)
				dirtyFlags->navMesh = true;
		}
	}
};

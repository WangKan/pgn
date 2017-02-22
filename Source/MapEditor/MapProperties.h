#include "Options.h"
#include "Utilities.h"

using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms::Design;
using namespace System::Xml;
using namespace System::Xml::Schema;
using namespace System::Xml::Serialization;

public ref class MapProperties
{
	Options^ options;
	Dictionary<String^, SceneEntityProperties^>^ _entities;

public:
	MapProperties()
	{
		_entities = gcnew Dictionary<String^, SceneEntityProperties^>;
	}

	MapProperties(Options^ options) : MapProperties()
	{
		this->options = options;
	}

	void copy(MapProperties^ props)
	{
		entities->Clear();
		for each(KeyValuePair<String^, SceneEntityProperties^> kvp in props->entities)
			entities->Add(kvp.Key, kvp.Value);
	}

	[CategoryAttribute("实体"), DisplayNameAttribute("实体")]
	property Dictionary<String^, SceneEntityProperties^>^  entities
	{
		Dictionary<String^, SceneEntityProperties^>^  get() { return _entities; }
	}
};


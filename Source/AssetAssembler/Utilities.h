using namespace System;

public ref class Utilities
{
public:
	static String^ getFileName(String^ fullName, String^ ext)
	{
		String^ name = fullName->Substring(fullName->LastIndexOf('\\') + 1);

		int startSuffix = name->LastIndexOf(".");
		String^ suffix = name->Substring(startSuffix != -1 ? startSuffix : name->Length);

		if (String::Compare(suffix, ext, true) == 0)
			name = name->Substring(0, name->Length - suffix->Length);

		return name;
	}
};

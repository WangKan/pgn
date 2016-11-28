#include <Windows.h>
#include <max.h>
#include "AnimExporter.h"
#include "MeshExporter.h"
#include "NavExporter.h"

HINSTANCE hInstance = 0;
 
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID /*lpvReserved*/)
{
  if( fdwReason == DLL_PROCESS_ATTACH )
  {
     // Hang on to this DLL's instance handle.
     hInstance = hinstDLL;
     DisableThreadLibraryCalls(hInstance);
  }
  return(TRUE);
}

const TCHAR* LibDescription()
{
	return L"PGN Asset File (.pnm, .pna) Exporter";
}

ClassDesc* classDescs[] =
{
	  meshExporterDesc
	, animExporterDesc
	, navExporterDesc
};

int LibNumberClasses()
{
	return sizeof(classDescs) / sizeof(ClassDesc*);
}

ClassDesc* LibClassDesc(int i)
{
	return classDescs[i];
}

ULONG LibVersion()
{
  return Get3DSMAXVersion();
}

int LibInitialize()
{
  // TODO: Perform initialization here.
  return TRUE;
}

int LibShutdown()
{
  // TODO: Perform uninitialization here.
  return TRUE;
}

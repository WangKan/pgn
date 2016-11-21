#include <cstring>
#include <PGN/Platform/outputDebugInfo.h>
#import <UIKit/UIKit.h>
#include "GLInfo.h"

void GLInfo::addRef()
{
    EAGLContext* rc = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    [EAGLContext setCurrentContext:rc];
    
    if(refCount++ == 0)
    {
        glInfo.vendor = glGetString(GL_VENDOR);
        glInfo.renderer = glGetString(GL_RENDERER);
        glInfo.version = glGetString(GL_VERSION);
        glInfo.shadingLanguageVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
        glGetIntegerv(GL_NUM_EXTENSIONS, &glInfo.numExt);
        glInfo.extensions = debug_new const unsigned char*[glInfo.numExt];
    }

	_outputDebugInfo(glInfo.vendor);
	_outputDebugInfo("\n");
	_outputDebugInfo(glInfo.renderer);
	_outputDebugInfo("\n");
	_outputDebugInfo(glInfo.version);
	_outputDebugInfo("\n");
	_outputDebugInfo(glInfo.shadingLanguageVersion);
	_outputDebugInfo("\n");

	for (int i = 0; i < glInfo.numExt; i++)
	{
		glInfo.extensions[i] = glGetStringi(GL_EXTENSIONS, i);
		_outputDebugInfo(glInfo.extensions[i]);
		_outputDebugInfo("\n");
	}

	struct GLExtension
	{
		char* name;
		bool* supported;
	};

	GLExtension extensions[] =
	{
		"GL_EXT_texture_filter_anisotropic", &glInfo.EXT_texture_filter_anisotropic
	};

	for (int i = 0; i < sizeof(extensions) / sizeof(GLExtension); i++)
	{
		bool supported = false;
		for (int j = 0; j < glInfo.numExt; j++)
		{
			if (!strcmp(extensions[i].name, (char*)glInfo.extensions[j]))
			{
				supported = true;
				break;
			}
		}
		*extensions[i].supported = supported;
	}
    
    [EAGLContext setCurrentContext:nil];
}

void GLInfo::release()
{
    if(--refCount == 0)
    {
        delete[] glInfo.extensions;
    }
}

GLInfo glInfo;

#include "WGL.h"
#include <gl/wglext.h>
#include <PGN/Common/debug_new.h>
#define PGN_STATIC_IMPORT
#include <PGN/Platform/outputDebugInfo.h>
#undef PGN_STATIC_IMPORT

PFNGLACTIVETEXTUREPROC					glActiveTexture;
PFNGLATTACHSHADERPROC					glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC				glBindAttribLocation;
PFNGLBINDBUFFERPROC						glBindBuffer;
PFNGLBINDBUFFERBASEPROC					glBindBufferBase;
PFNGLBINDBUFFERRANGEPROC				glBindBufferRange;
PFNGLBINDFRAMEBUFFERPROC				glBindFramebuffer;
PFNGLBINDRENDERBUFFERPROC				glBindRenderbuffer;
PFNGLBINDSAMPLERPROC					glBindSampler;
PFNGLBLENDCOLORPROC						glBlendColor;
PFNGLBLENDEQUATIONSEPARATEIPROC			glBlendEquationSeparatei;
PFNGLBLENDFUNCSEPARATEIPROC				glBlendFuncSeparatei;
PFNGLBLENDEQUATIONSEPARATEPROC			glBlendEquationSeparate;
PFNGLBLENDFUNCSEPARATEPROC				glBlendFuncSeparate;
PFNGLBLITFRAMEBUFFERPROC				glBlitFramebuffer;
PFNGLBUFFERDATAPROC						glBufferData;
PFNGLBUFFERSUBDATAPROC					glBufferSubData;
PFNGLCHECKFRAMEBUFFERSTATUSPROC			glCheckFramebufferStatus;
PFNGLCLEARDEPTHFPROC					glClearDepthf;
PFNGLCOMPILESHADERPROC					glCompileShader;
PFNGLCOMPRESSEDTEXIMAGE2DPROC			glCompressedTexImage2D;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC		glCompressedTexSubImage2D;
PFNGLCREATEPROGRAMPROC					glCreateProgram;
PFNGLCREATESHADERPROC					glCreateShader;
PFNGLDELETEBUFFERSPROC					glDeleteBuffers;
PFNGLDELETEFRAMEBUFFERSPROC				glDeleteFramebuffers;
PFNGLDELETEPROGRAMPROC					glDeleteProgram;
PFNGLDELETERENDERBUFFERSPROC			glDeleteRenderbuffers;
PFNGLDELETESAMPLERSPROC					glDeleteSamplers;
PFNGLDELETESHADERPROC					glDeleteShader;
PFNGLDELETESYNCPROC						glDeleteSync;
PFNGLDEPTHRANGEFPROC					glDepthRangef;
PFNGLDETACHSHADERPROC					glDetachShader;
PFNGLDISABLEIPROC						glDisablei;
PFNGLDISABLEVERTEXATTRIBARRAYPROC		glDisableVertexAttribArray;
PFNGLDRAWBUFFERSPROC					glDrawBuffers;
PFNGLDRAWELEMENTSINSTANCEDPROC			glDrawElementsInstanced;
PFNGLENABLEIPROC						glEnablei;
PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
PFNGLFENCESYNCPROC						glFenceSync;
PFNGLFRAMEBUFFERRENDERBUFFERPROC		glFramebufferRenderbuffer;
PFNGLFRAMEBUFFERTEXTURE2DPROC			glFramebufferTexture2D;
PFNGLGENBUFFERSPROC						glGenBuffers;
PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
PFNGLGENRENDERBUFFERSPROC				glGenRenderbuffers;
PFNGLGENSAMPLERSPROC					glGenSamplers;
PFNGLGETACTIVEATTRIBPROC				glGetActiveAttrib;
PFNGLGETACTIVEUNIFORMPROC				glGetActiveUniform;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC		glGetActiveUniformBlockiv;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC		glGetActiveUniformBlockName;
PFNGLGETATTRIBLOCATIONPROC				glGetAttribLocation;
PFNGLGETPROGRAMBINARYPROC				glGetProgramBinary;
PFNGLGETPROGRAMINFOLOGPROC				glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC					glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC				glGetShaderInfoLog;
PFNGLGETSHADERIVPROC					glGetShaderiv;
PFNGLGETSHADERSOURCEPROC				glGetShaderSource;
PFNGLGETSTRINGIPROC						glGetStringi;
PFNGLGETSYNCIVPROC						glGetSynciv;
PFNGLGETUNIFORMBLOCKINDEXPROC			glGetUniformBlockIndex;
PFNGLGETUNIFORMLOCATIONPROC				glGetUniformLocation;
PFNGLLINKPROGRAMPROC					glLinkProgram;
PFNGLMAPBUFFERRANGEPROC					glMapBufferRange;
PFNGLPROGRAMBINARYPROC					glProgramBinary;
PFNGLPROGRAMPARAMETERIPROC				glProgramParameteri;
PFNGLRENDERBUFFERSTORAGEPROC			glRenderbufferStorage;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC	glRenderbufferStorageMultisample;
PFNGLSAMPLECOVERAGEPROC					glSampleCoverage;
PFNGLSAMPLERPARAMETERFPROC				glSamplerParameterf;
PFNGLSAMPLERPARAMETERIPROC				glSamplerParameteri;
PFNGLSHADERSOURCEPROC					glShaderSource;
PFNGLSTENCILFUNCSEPARATEPROC			glStencilFuncSeparate;
PFNGLSTENCILOPSEPARATEPROC				glStencilOpSeparate;
PFNGLUNIFORM1IPROC						glUniform1i;
PFNGLUNIFORMBLOCKBINDINGPROC			glUniformBlockBinding;
PFNGLUNMAPBUFFERPROC					glUnmapBuffer;
PFNGLUSEPROGRAMPROC						glUseProgram;
PFNGLVERTEXATTRIBDIVISORPROC			glVertexAttribDivisor;
PFNGLVERTEXATTRIBIPOINTERPROC			glVertexAttribIPointer;
PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
PFNWGLCHOOSEPIXELFORMATARBPROC			wglChoosePixelFormatARB;
PFNWGLSWAPINTERVALEXTPROC				wglSwapIntervalEXT;

static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

WGL::WGL()
{
	refCount = 0;
}

void WGL::addRef()
{
	if (refCount++)
		return;

	char wndClassName[] = "dummy window";
	WNDCLASS wc =
	{
		NULL,
		wndProc,
		0, 0,
		GetModuleHandle(NULL),
		NULL, NULL,
		NULL,
		NULL,
		wndClassName
	};
	RegisterClass(&wc);
	HWND hWnd = CreateWindow(wndClassName, "", WS_OVERLAPPED, 0, 0, 0, 0, HWND_DESKTOP, NULL, GetModuleHandle(NULL), NULL);
	HDC hDC = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
		1,                     // version number  
		PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		24,                    // 24-bit color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		0,                     // no alpha buffer  
		0,                     // shift bit ignored  
		0,                     // no accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		32,                    // 32-bit z-buffer      
		0,                     // no stencil buffer  
		0,                     // no auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	};
	SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
	HGLRC hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)wglGetProcAddress("glBindBufferBase");
	glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)wglGetProcAddress("glBindBufferRange");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
	glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");
	glBlendColor = (PFNGLBLENDCOLORPROC)wglGetProcAddress("glBlendColor");
	glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)wglGetProcAddress("glBlendEquationSeparate");
	glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)wglGetProcAddress("glBlendEquationSeparatei");
	glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)wglGetProcAddress("glBlendFuncSeparate");
	glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)wglGetProcAddress("glBlendFuncSeparatei");
	glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)wglGetProcAddress("glBlitFramebuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
	glClearDepthf = (PFNGLCLEARDEPTHFPROC)wglGetProcAddress("glClearDepthf");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)wglGetProcAddress("glCompressedTexImage2D");
	glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)wglGetProcAddress("glCompressedTexSubImage2D");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
	glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)wglGetProcAddress("glDeleteSamplers");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glDeleteSync = (PFNGLDELETESYNCPROC)wglGetProcAddress("glDeleteSync");
	glDepthRangef = (PFNGLDEPTHRANGEFPROC)wglGetProcAddress("glDepthRangef");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glDisablei = (PFNGLDISABLEIPROC)wglGetProcAddress("glDisablei");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");
	glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress("glDrawElementsInstanced");
	glEnablei = (PFNGLENABLEIPROC)wglGetProcAddress("glEnablei");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glFenceSync = (PFNGLFENCESYNCPROC)wglGetProcAddress("glFenceSync");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	glGenSamplers = (PFNGLGENSAMPLERSPROC)wglGetProcAddress("glGenSamplers");
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)wglGetProcAddress("glGetActiveAttrib");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
	glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)wglGetProcAddress("glGetActiveUniformBlockiv");
	glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)wglGetProcAddress("glGetActiveUniformBlockName");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)wglGetProcAddress("glGetProgramBinary");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)wglGetProcAddress("glGetShaderSource");
	glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
	glGetSynciv = (PFNGLGETSYNCIVPROC)wglGetProcAddress("glGetSynciv");
	glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)wglGetProcAddress("glGetUniformBlockIndex");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");
	glProgramBinary = (PFNGLPROGRAMBINARYPROC)wglGetProcAddress("glProgramBinary");
	glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)wglGetProcAddress("glProgramParameteri");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
	glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)wglGetProcAddress("glRenderbufferStorageMultisample");
	glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)wglGetProcAddress("glSampleCoverage");
	glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)wglGetProcAddress("glSamplerParameterf");
	glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)wglGetProcAddress("glSamplerParameteri");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)wglGetProcAddress("glStencilFuncSeparate");
	glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)wglGetProcAddress("glStencilOpSeparate");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)wglGetProcAddress("glUniformBlockBinding");
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)wglGetProcAddress("glVertexAttribDivisor");
	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

	glInfo.vendor = glGetString(GL_VENDOR);
	glInfo.renderer = glGetString(GL_RENDERER);
	glInfo.version = glGetString(GL_VERSION);
	glInfo.shadingLanguageVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	glGetIntegerv(GL_NUM_EXTENSIONS, &glInfo.numExt);
	glInfo.extensions = debug_new const unsigned char*[glInfo.numExt];

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

	for (auto ext : extensions)
	{
		bool supported = false;
		for (int i = 0; i < glInfo.numExt; i++)
		{
			if (!strcmp(ext.name, (char*)glInfo.extensions[i]))
			{
				supported = true;
				break;
			}
		}
		*ext.supported = supported;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
	DestroyWindow(hWnd);
	UnregisterClass(wndClassName, NULL);
}

void WGL::release()
{
	if (--refCount)
		return;

	delete[] glInfo.extensions;
}

WGL wgl;

pgn::GLInfo* pgn::getGLInfo()
{
	return &wgl.glInfo;
}

char* pgn::getGLSLVersionDecl()
{
	return "#version 430";
}

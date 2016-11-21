#include <cassert>
#include <iostream>
#include <PGN/Common/debug_new.h>
#include <PGN/Platform/outputDebugInfo.h>
#include <PGN/Platform/UI/GL.h>
#include <PGN/Utilities/Text.h>
#include "RenderingSystem.h"
#include "Shader.h"

const unsigned types[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

Shader::Shader(pgn::ShaderDesc* desc)
{
	pgn::Text* src = pgn::Text::create(128 * 1024);
	src->setText(desc->src);

	char name[128];
	int bindingPoint;

	while (src->extract("(\\w+)@(\\d+)", name, &bindingPoint))
	{
		uniformBlockBindingPointMap[name] = bindingPoint;
		src->replace("@(\\d+)", "");
	}

	while (src->extract("(\\w+)#(\\d+)", name, &bindingPoint))
	{
		samplerVariableBindingPointMap[name] = bindingPoint;
		src->replace("#(\\d+)", "");
	}

	type = types[desc->type];
	shader = glCreateShader(type);

	char* s[] = {pgn::getGLSLVersionDecl(), "\n", src->getText()};
	glShaderSource(shader, sizeof(s)/sizeof(char*), s, NULL);

	src->destroy();

	const int bufSize = 4096;
	char str[bufSize];

	glGetShaderSource(shader, bufSize, NULL, str);
	_outputDebugInfo(str);

	glCompileShader(shader);
	glGetShaderInfoLog(shader, bufSize, NULL, str);
	_outputDebugInfo(str);

	int compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	assert(compileStatus == GL_TRUE);
}

void Shader::dispose()
{
	glDeleteShader(shader);
}

pgn::Shader* RenderingSystem::createShader(pgn::ShaderDesc* desc)
{
	return debug_new Shader(desc);
}

void Shader::_free()
{
	delete this;
}

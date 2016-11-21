#include <cassert>
#include <PGN/Common/Blob.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Platform/outputDebugInfo.h>
#include <PGN/Platform/UI/GL.h>
#include <set>
#include "Program.h"
#include "RenderingSystem.h"
#include "Shader.h"

const int maxNameLen = 31;

struct UniformBlockBinding
{
	char uniformBlockName[maxNameLen + 1];
	int bindingPoint;
};

const int maxNumUniformBlocks = 5;
const int maxNumSamplerVariables = 8;

struct ProgramBlob
{
	int numUniformBlocks;
	UniformBlockBinding uniformBlockBindings[maxNumUniformBlocks];

	int numSamplerVariables;
	char samplerVariableNames[maxNumSamplerVariables][maxNameLen + 1];

	GLenum binaryFormat;
	int binaryLength;
	char binary[1];
};

void Program::init_create()
{
	program = glCreateProgram();
}

void Program::init_link(Shader* vs, Shader* ps)
{
	glAttachShader(program, vs->shader);
	glAttachShader(program, ps->shader);
	glLinkProgram(program);

	const int logBufSize = 4096;
	char logBuf[logBufSize];

	glGetProgramInfoLog(program, logBufSize, NULL, logBuf);
	_outputDebugInfo(logBuf);

	int linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	assert(linkStatus == GL_TRUE);

	uniformBlockBindingPointMap = vs->uniformBlockBindingPointMap;
	uniformBlockBindingPointMap.insert(ps->uniformBlockBindingPointMap.begin(), ps->uniformBlockBindingPointMap.end());

	auto map = vs->samplerVariableBindingPointMap;
	map.insert(ps->samplerVariableBindingPointMap.begin(), ps->samplerVariableBindingPointMap.end());

	samplerVariables.resize(map.size());

	for (auto entry : map)
	{
		samplerVariables[entry.second].name = entry.first;
	}
}

bool Program::init_link(pgn::Blob* _programBlob)
{
	ProgramBlob* programBlob = (ProgramBlob*)_programBlob->getBuffer();
	glProgramBinary(program, programBlob->binaryFormat, programBlob->binary, programBlob->binaryLength);

	const int logBufSize = 4096;
	char logBuf[logBufSize];

	glGetProgramInfoLog(program, logBufSize, NULL, logBuf);
	_outputDebugInfo(logBuf);

	int linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus != GL_TRUE)
		return false;

	UniformBlockBinding* bindings = programBlob->uniformBlockBindings;
	for (int i = 0; i < programBlob->numUniformBlocks; i++)
	{
		uniformBlockBindingPointMap[bindings[i].uniformBlockName] = bindings[i].bindingPoint;
	}

	samplerVariables.resize(programBlob->numSamplerVariables);
	for (int i = 0; i < programBlob->numSamplerVariables; i++)
	{
		samplerVariables[i].name = programBlob->samplerVariableNames[i];
	}

	return true;
}

void Program::init_doTheRest()
{
	for (auto entry : uniformBlockBindingPointMap)
	{
		int index = glGetUniformBlockIndex(program, entry.first.c_str());
		glUniformBlockBinding(program, index, entry.second);
	}

	int numAttribs;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttribs);

	for (int i = 0; i < numAttribs; i++)
	{
		const int bufSize = 64;
		char name[bufSize];
		int size;
		unsigned type;

		glGetActiveAttrib(program, i, bufSize, NULL, &size, &type, name);
		attribLocationMap[name] = glGetAttribLocation(program, name);
	}

	int numUniforms;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

	for (int i = 0; i < numUniforms; i++)
	{
		const int bufSize = 64;
		char name[bufSize];
		int size;
		unsigned type;

		glGetActiveUniform(program, i, bufSize, NULL, &size, &type, name);

		if (type == GL_SAMPLER_2D || type == GL_UNSIGNED_INT_SAMPLER_2D)
		{
			int location = glGetUniformLocation(program, name);

			int bindingPoint;

			for (int i = 0; i < samplerVariables.size(); i++)
			{
				if (samplerVariables[i].name == name)
				{
					bindingPoint = i;
					break;
				}
			}

			samplerVariables[bindingPoint].location = location;
		}
	}
}

void Program::init(Shader* vs, Shader* ps)
{
	init_create();
	init_link(vs, ps);
	init_doTheRest();
}

bool Program::init(pgn::Blob* programBlob)
{
	init_create();
	if (!init_link(programBlob))
	{
		dispose();
		return false;
	}
	init_doTheRest();
	return true;
}

pgn::Blob* Program::serialize()
{
	int binaryLength;
	glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &binaryLength);

	pgn::Blob* blob = pgn::Blob::create(offsetof(ProgramBlob, binary) + binaryLength);
	ProgramBlob* programBlob = (ProgramBlob*)blob->getBuffer();

	assert(uniformBlockBindingPointMap.size() <= maxNumUniformBlocks);

	programBlob->numUniformBlocks = (int)uniformBlockBindingPointMap.size();
	UniformBlockBinding* binding = programBlob->uniformBlockBindings;
	for (auto entry : uniformBlockBindingPointMap)
	{
		assert(entry.first.length() <= maxNameLen);
		strcpy(binding->uniformBlockName, entry.first.c_str());
		binding->bindingPoint = entry.second;
		binding++;
	}

	programBlob->numSamplerVariables = (int)samplerVariables.size();
	for (int i = 0; i < samplerVariables.size(); i++)
	{
		assert(samplerVariables[i].name.length() <= maxNameLen);
		strcpy(programBlob->samplerVariableNames[i], samplerVariables[i].name.c_str());
	}

	programBlob->binaryLength = binaryLength;
	glGetProgramBinary(program, binaryLength, &programBlob->binaryLength, &programBlob->binaryFormat, programBlob->binary);

	return blob;
}

void Program::dispose()
{
	glDeleteProgram(program);
}

pgn::Program* RenderingSystem::createProgram(pgn::Shader* vs, pgn::Shader* ps)
{
	Program* program = debug_new Program;
	program->init((Shader*)vs, (Shader*)ps);
	return program;
}

pgn::Program* RenderingSystem::createProgram(pgn::Blob* programBlob)
{
	Program* program = debug_new Program;
	if (!program->init(programBlob))
	{
		delete program;
		program = 0;
	}
	return program;
}

void Program::_free()
{
	delete this;
}

int Program::getNumTextureVariables()
{
	return (int)samplerVariables.size();
}

const char* Program::getTextureVariableName(int i)
{
	return samplerVariables[i].name.c_str();
}

pgn::Blob* RenderingSystem::serialize(pgn::Program* program)
{
	return ((Program*)program)->serialize();
}

void RenderingSystem::setProgram(pgn::Program* program)
{
	if (program)
	{
		glUseProgram(((Program*)program)->program);

		auto& samplerVariables = ((Program*)program)->samplerVariables;

		for (unsigned i = 0; i < samplerVariables.size(); i++)
			glUniform1i(samplerVariables[i].location, i);
	}
	else
	{
		glUseProgram(0);
	}
}

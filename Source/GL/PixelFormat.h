#pragma once
#include <PGN/Platform/UI/GL.h>

struct PixelFormat
{
	unsigned internalFormat;
	unsigned format;
	unsigned type;
	unsigned colorRenderable	: 1;
	unsigned depthRenderable	: 1;
	unsigned stencilRenderable	: 1;
	unsigned compressed			: 1;
	int blockWidth;
	int blockHeight;
	int bitsPerPixel;
};

const PixelFormat formats[] =
{
//	internalFormat									format											type								colorRend	depthRend	stencilRend	compr	blockW	blockH	bpp
	GL_R8,											GL_RED,											GL_UNSIGNED_BYTE,					1,			0,			0,			0,		1,		1,		8,		// R8
	GL_RG8,											GL_RG,											GL_UNSIGNED_BYTE,					1,			0,			0,			0,		1,		1,		16,		// RG8
	GL_RGB8,										GL_RGB,											GL_UNSIGNED_BYTE,					1,			0,			0,			0,		1,		1,		24,		// RGB8
	GL_RGBA8,										GL_RGBA,										GL_UNSIGNED_BYTE,					1,			0,			0,			0,		1,		1,		32,		// RGBA8
	GL_R8UI,										GL_RED_INTEGER,									GL_UNSIGNED_BYTE,					1,			0,			0,			0,		1,		1,		8,		// R8UI
	GL_RG8UI,										GL_RG_INTEGER,									GL_UNSIGNED_BYTE,					1,			0,			0,			0,		1,		1,		16,		// RG8UI
	GL_RGB8UI,										GL_RGB_INTEGER,									GL_UNSIGNED_BYTE,					1,			0,			0,			0,		1,		1,		24,		// RGB8UI
	GL_RGBA8UI,										GL_RGBA_INTEGER,								GL_UNSIGNED_BYTE,					1,			0,			0,			0,		1,		1,		32,		// RGBA8UI
	GL_R16F,										GL_RED,											GL_HALF_FLOAT,						0,			0,			0,			0,		1,		1,		16,		// R16F
	GL_RG16F,										GL_RG,											GL_HALF_FLOAT,						0,			0,			0,			0,		1,		1,		32,		// RG16F
	GL_RGB16F,										GL_RGB,											GL_HALF_FLOAT,						0,			0,			0,			0,		1,		1,		48,		// RGB16F
	GL_RGBA16F,										GL_RGBA,										GL_HALF_FLOAT,						0,			0,			0,			0,		1,		1,		64,		// RGBA16F
	GL_R32F,										GL_RED,											GL_FLOAT,							0,			0,			0,			0,		1,		1,		32,		// R32F
	GL_RG32F,										GL_RG,											GL_FLOAT,							0,			0,			0,			0,		1,		1,		64,		// RG32F
	GL_RGB32F,										GL_RGB,											GL_FLOAT,							0,			0,			0,			0,		1,		1,		96,		// reserved_for_RGB32F
	GL_RGBA32F,										GL_RGBA,										GL_FLOAT,							0,			0,			0,			0,		1,		1,		128,	// reserved_for_RGBA32F

	GL_DEPTH24_STENCIL8,							GL_DEPTH_STENCIL,								GL_UNSIGNED_INT_24_8,				0,			1,			1,			0,		1,		1,		32,		// D24_S8
	GL_DEPTH32F_STENCIL8,							GL_DEPTH_STENCIL,								GL_FLOAT_32_UNSIGNED_INT_24_8_REV,	0,			1,			1,			0,		1,		1,		64,		// reserved_for_D32F_X24_S8
	GL_DEPTH_COMPONENT32F,							GL_DEPTH_COMPONENT,								GL_FLOAT,							0,			1,			0,			0,		1,		1,		32,		// reserved_for_D32F

	GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,				GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,				0,									0,			0,			0,			1,		4,		4,		4,		// DXT1
	GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,				GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,				0,									0,			0,			0,			1,		4,		4,		8,		// DXT3
	GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,				GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,				0,									0,			0,			0,			1,		4,		4,		8,		// DXT5
	GL_COMPRESSED_RGB8_ETC2,						GL_COMPRESSED_RGB8_ETC2,						0,									0,			0,			0,			1,		4,		4,		4,		// RGB8_ETC2
	GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,	GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,	0,									0,			0,			0,			1,		4,		4,		4,		// RGB8_ETC2
	GL_COMPRESSED_RGBA8_ETC2_EAC,					GL_COMPRESSED_RGBA8_ETC2_EAC,					0,									0,			0,			0,			1,		4,		4,		8		// RGBA8_ETC2_EAC
};

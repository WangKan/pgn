#define PGN_DLL_EXPORT
#include <PGN/FileStream/ZipFileStream.h>
#undef PGN_DLL_EXPORT

#include <cassert>
#include <fstream>
#include <PGN/Common/debug_new.h>
#include <PGN/FileStream/FileStream.h>
#include <PGN/Platform/FileStream.h>
#include <unzip.h>

class ZipFileStream : public pgn::FileStream
{
	std::fstream f;
	unzFile zip;
	static const int maxNameLen = 511;
	char fullName[maxNameLen + 1];
	size_t workingDirStrLen;
	char* subNameBaseAddr;
public:
	ZipFileStream(const char workingDir[]);
	virtual void open(const char fileName[], OpenMode mode);
	virtual bool isOpen();
	virtual void close();
	virtual long long getSize();
	virtual void read(void* buf, std::streamsize numBytes);
	virtual void write(void* buf, std::streamsize numBytes);
};

ZipFileStream::ZipFileStream(const char workingDir[])
{
	zip = 0;
	workingDirStrLen = strlen(workingDir);
	assert(workingDirStrLen <= maxNameLen);
	strcpy(fullName, workingDir);
	subNameBaseAddr = fullName + workingDirStrLen;
}

void ZipFileStream::open(const char fileName[], OpenMode mode)
{
	const char* name;

	if (fileName[0] == '^')
	{
		name = fileName + 1;
	}
	else if (!pgn::isAbsolutePath(fileName))
	{
		assert(workingDirStrLen + strlen(fileName) <= maxNameLen);
		strcpy(subNameBaseAddr, fileName);
		name = fullName;
	}
	else
	{
		name = fileName;
	}

	char buf[maxNameLen + 1];
	char* zipFileName = 0;
	char* fileNameInZipFile = 0;

	size_t len = strlen(name);
	for (int i = 0; i < len; i++)
	{
		if (name[i] == '>')
		{
			strcpy(buf, name);
			buf[i] = 0;
			zipFileName = buf;
			fileNameInZipFile = &buf[i + 1];
			break;
		}
	}

	if (zipFileName)
	{
		assert((mode & in) ^ (mode & out));

		if (mode & in)
		{
			zip = unzOpen64(zipFileName);

			if (zip)
			{
				if (unzLocateFile(zip, fileNameInZipFile, 0) == UNZ_OK)
				{
					unzOpenCurrentFile(zip);
				}
				else
				{
					unzClose(zip);
					zip = 0;
				}
			}
		}
		else
		{
			assert(0);
		}
	}
	else
	{
		std::ios_base::openmode _mode = std::ios_base::binary;

		if (mode & in)
			_mode |= std::ios_base::in;

		if (mode & out)
			_mode |= std::ios_base::out;

		f.open(name, _mode);
	}
}

bool ZipFileStream::isOpen()
{
	return zip || f.is_open();
}

void ZipFileStream::close()
{
	if (zip)
	{
		unzCloseCurrentFile(zip);
		unzClose(zip);
		zip = 0;
	}
	else
	{
		f.close();
	}
}

long long ZipFileStream::getSize()
{
	long long size;

	if (zip)
	{
		unz_file_info64 info;
		unzGetCurrentFileInfo64(zip, &info, 0, 0, 0, 0, 0, 0);
		size = info.uncompressed_size;
	}
	else
	{
		f.seekg(0, std::ios_base::end);
		size = f.tellg();
		f.seekg(0, std::ios_base::beg);
	}

	return size;
}

void ZipFileStream::read(void* buf, std::streamsize numBytes)
{
	if (zip)
		unzReadCurrentFile(zip, buf, numBytes);
	else
		f.read((char*)buf, numBytes);
}

void ZipFileStream::write(void* buf, std::streamsize numBytes)
{
	f.write((char*)buf, numBytes);
}

pgn::FileStream* pgn::createZipFileStream(const char workingDir[])
{
	return debug_new::ZipFileStream(workingDir);
}

void pgn::destroyZipFileStream(FileStream* f)
{
	delete f;
}

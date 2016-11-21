/*

	约翰·卡马克是个很晚才开口说话的孩子。他父母的担心一直持续到1971年的一天，这个15个月大的男孩儿
	拿着一块儿海绵蹒跚着走进客厅，说出不光是一个单词而是一个完整的句子：“爸爸，你的洗澡巾。”就好像
	他一直不屑于说单个儿的词，而非要等到能用它们组合出完整的句子才肯开口。

																	———— 《DOOM启世录》

*/

#define PGN_DLL_EXPORT
#include <PGN/Utilities/ResourceManager/AsyncLoader.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <PGN/FileStream/FileStream.h>
#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Platform/outputDebugInfo.h>
#include <PGN/Platform/UI/RenderingContext.h>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/CircularBuffer.h>
#include <PGN/Utilities/Pipeline.h>
#undef PGN_STATIC_IMPORT
#include <PGN/Utilities/ResourceManager/Asset.h>
#include <PGN/Utilities/ResourceManager/AssetFactory.h>
#include <PGN/Utilities/ResourceManager/AssetLoader.h>

enum LoadingStatus
{
	loading, ready, failed, unloading
};

struct Entry
{
	pgn::FileStream* f;
	char name[256];
	pgn::Asset* asset;
	LoadingStatus status;
	void* rawData;
	int sizeRawData;
};

class ReadingStage : public pgn::PipelineStage
{
	pgn::CircularBuffer* rawDataBuf;

public:
	ReadingStage(pgn::CircularBuffer* rawDataBuf)
	{
		this->rawDataBuf = rawDataBuf;
	}

	virtual void onBegin()
	{
	}

	virtual void onEnd()
	{
	}

	virtual void process(void* p)
	{
		Entry* entry = (Entry*)p;

		if (entry->status == loading)
		{
			pgn::FileStream* f = entry->f;
			f->open(entry->name, pgn::FileStream::in);

			if (f->isOpen())
			{
				entry->sizeRawData = (int)f->getSize();
				entry->rawData = rawDataBuf->alloc(entry->sizeRawData);
				f->read(entry->rawData, entry->sizeRawData);
				f->close();
			}
			else
			{
				entry->status = failed;
			}
		}
	}
};

class CookingStage : public pgn::PipelineStage
{
	virtual void onBegin()
	{
	}

	virtual void onEnd()
	{
	}

	virtual void process(void* p)
	{
		Entry* entry = (Entry*)p;

		if (entry->status == loading)
			if (!entry->asset->cook(entry->rawData))
				entry->status = failed;
	}
};

class SubmittingStage : public pgn::PipelineStage
{
	pgn::CircularBuffer* rawDataBuf;

	pgn::RenderingContext* rc;
	pgn::RenderingSystem* rs;
	pgn::Display display;

public:
	SubmittingStage(pgn::CircularBuffer* rawDataBuf, pgn::RenderingContext* rc, pgn::RenderingSystem* rs, pgn::Display display)
	{
		this->rawDataBuf = rawDataBuf;
		this->rc = rc;
		this->rs = rs;
		this->display = display;
	}

	virtual void onBegin()
	{
		if (rc) rc->beginDraw(display, 0);
	}

	virtual void onEnd()
	{
		if (rc) rc->endDraw();
	}

	virtual void process(void* p)
	{
		Entry* entry = (Entry*)p;

		if (entry->status == loading)
		{
			entry->status = entry->asset->submit(entry->rawData, rs) ? ready : failed;
			rawDataBuf->_free(entry->sizeRawData);
		}
		else if (entry->status == unloading)
		{
			entry->asset->unload(rs);
		}
	}
};

class AsyncLoader : public pgn::AssetLoader
{
public:
	ReadingStage* readingStage;
	CookingStage* cookingStage;
	SubmittingStage* submittingStage;
	pgn::Pipeline* pipeline;
	pgn::CircularBuffer* rawDataBuf;

	pgn::RenderingContext* rc;
	pgn::RenderingSystem* rs;
	pgn::Display display;

	AsyncLoader(pgn::RenderingContext* rc = 0, pgn::RenderingSystem* rs = 0, pgn::Display display = 0);
	~AsyncLoader();
	virtual bool load(pgn::FileStream* f, char name[], pgn::Asset* asset);
	virtual bool unload(pgn::Asset* asset);
	virtual pgn::Asset* getGarbage(int maxTry);
	virtual void finish();
};

AsyncLoader::AsyncLoader(pgn::RenderingContext* rc, pgn::RenderingSystem* rs, pgn::Display display)
{
	rawDataBuf = pgn::CircularBuffer::create(32 * 1024 * 1024);
	readingStage = debug_new ReadingStage(rawDataBuf);
	cookingStage = debug_new CookingStage;
	submittingStage = debug_new SubmittingStage(rawDataBuf, rc, rs, display);

	pgn::PipelineStage* stages[] =
	{
		readingStage, cookingStage, submittingStage
	};

	pipeline = pgn::Pipeline::create(sizeof(Entry), 128, sizeof(stages) / sizeof(pgn::PipelineStage*), stages, false);
}

AsyncLoader::~AsyncLoader()
{
	pipeline->destroy();
	delete readingStage;
	delete cookingStage;
	delete submittingStage;
	rawDataBuf->destroy();
}

bool AsyncLoader::load(pgn::FileStream* f, char name[], pgn::Asset* asset)
{
	if (strlen(name) >= sizeof(((Entry*)0)->name))
	{
		char buf[512];
		sprintf(buf, "文件路径过长%s\n", name);
		pgn::outputDebugInfo(buf);
		return true;
	}

	Entry entry;
	entry.f = f;
	strcpy(entry.name, name);
	entry.asset = asset;
	entry.status = loading;
	return pipeline->put(&entry);
}

bool AsyncLoader::unload(pgn::Asset* asset)
{
	Entry entry;
	entry.asset = asset;
	entry.status = unloading;
	return pipeline->put(&entry);
}

pgn::Asset* AsyncLoader::getGarbage(int maxTry)
{
	pgn::Asset* garbage = 0;

	for (int i = 0; i < maxTry && !garbage; i++)
	{
		Entry* entry = (Entry*)pipeline->get();

		if (!entry)
			break;

		if (entry->status == unloading)
			garbage = entry->asset;
	}

	return garbage;
}

void AsyncLoader::finish()
{
	pipeline->finish();
}

pgn::AssetLoader* pgn::createAsyncLoader()
{
	return debug_new AsyncLoader;
}

pgn::AssetLoader* pgn::createAsyncLoader(pgn::RenderingContext* rc, pgn::RenderingSystem* rs, pgn::Display display)
{
	return debug_new AsyncLoader(rc, rs, display);
}

void pgn::destroyAsyncLoader(pgn::AssetLoader* loader)
{
	delete (AsyncLoader*)loader;
}

static pgn::AssetLoader* defaultLoader = 0;
static int refCount = 0;

pgn::AssetLoader* pgn::getDefaultAsyncLoader()
{
	if (!defaultLoader)
		defaultLoader = createAsyncLoader();

	refCount++;
	return defaultLoader;
}

void pgn::releaseDefaultAsyncLoader()
{
	if (--refCount == 0)
		destroyAsyncLoader(defaultLoader);
}

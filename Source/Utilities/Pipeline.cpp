/*

	一次，卡马克找到了他在《创世纪》里的角色所对应的代码，他修改了那一段，给自己增加了一些额外的能力，
	这是他第一次体会到凭空创造东西的乐趣。而且，作为一名程序员，他不需要依赖于其他人，只要他的代码逻辑
	正确表述了他定义的规则，它就会工作得很好，一切都那么合理。

	一切————除了父母，卡马克想道。

																	———— 《DOOM启世录》

*/

#define PGN_DLL_EXPORT
#include <PGN/Utilities/Pipeline.h>
#undef PGN_DLL_EXPORT

#include <PGN/Common/debug_new.h>
#include <PGN/Platform/Thread/Signal.h>
#include <stddef.h>
#include <thread>
#include <vector>

class ProducerConsumer
{
public:
	int itemSize;
	char *buf, *bufEnd;
	ProducerConsumer* source;
	volatile char* p;
	pgn::Signal* newProductsSignal;

	void init(int itemSize, char* buf, char* bufEnd, ProducerConsumer* source)
	{
		this->itemSize = itemSize;
		this->buf = buf;
		this->bufEnd = bufEnd;
		this->source = source;
		p = buf;
		newProductsSignal = pgn::Signal::create();
	}

	void dispose()
	{
		newProductsSignal->destroy();
	}
};

class Stage : public ProducerConsumer
{
public:
	pgn::PipelineStage* callbacks;
	std::thread* thread;
	volatile bool cancellationPending;

	void threadFunc()
	{
		callbacks->onBegin();

		while(!cancellationPending)
		{
			source->newProductsSignal->wait();
			char* _p = (char*)p;

			while (_p != source->p)
			{
				callbacks->process(_p);
				_p += itemSize;

				if (_p == bufEnd)
					_p = buf;

				p = _p;
				newProductsSignal->raise();
			}
		}

		callbacks->onEnd();
	}

	void init(int itemSize, char* buf, char* bufEnd, ProducerConsumer* source, pgn::PipelineStage* callbacks)
	{
		ProducerConsumer::init(itemSize, buf, bufEnd, source);
		this->callbacks = callbacks;
		cancellationPending = false;
		thread = debug_new std::thread(&Stage::threadFunc, this);
	}

	void dispose()
	{
		cancellationPending = true;
		source->newProductsSignal->raise();
		thread->join();
		delete thread;
		ProducerConsumer::dispose();
	}
};

class Pipeline : public pgn::Pipeline
{
public:
	int itemSize;
	int sizeBuf;
	char *buf, *bufEnd;
	bool autoGet;
	ProducerConsumer beginning;
	ProducerConsumer end;
	std::vector<Stage> stages;

	Pipeline(int itemSize, int maxNumInFlightProducts, int numStages, pgn::PipelineStage* stageCallbacks[], bool autoGet)
	{
		this->itemSize = itemSize;
		sizeBuf = itemSize * (maxNumInFlightProducts + 1);
		buf = debug_new char[sizeBuf];
		bufEnd = buf + sizeBuf;
		this->autoGet = autoGet;

		ProducerConsumer* producer;

		beginning.init(itemSize, buf, bufEnd, 0);
		producer = &beginning;

		stages.resize(numStages);
		for(int i = 0; i < numStages; i++)
		{
			stages[i].init(itemSize, buf, bufEnd, producer, stageCallbacks[i]);
			producer = &stages[i];
		}

		if(!autoGet)
		{
			end.init(itemSize, buf, bufEnd, producer);
			producer = &end;
		}

		beginning.source = producer;
	}

	virtual void dispose()
	{
		for(int i = (int)stages.size() - 1; i >= 0; i--) // 竟然没有一个带符号的size_t
			stages[i].dispose();

		beginning.dispose();

		if(!autoGet)
			end.dispose();

		delete[] buf;
	}

	virtual void _free()
	{
		delete this;
	}

	virtual bool put(void* item)
	{
		char* p = (char*)beginning.p;

		ptrdiff_t d = beginning.source->p - p;

		if (d < 0)
			d += sizeBuf;

		if (d == itemSize)
			return false;

		memcpy(p, item, itemSize);

		p += itemSize;

		if(p == bufEnd)
			p = buf;

		beginning.p = p;
		beginning.newProductsSignal->raise();
		return true;
	}

	virtual void* get()
	{
		char* p = (char*)end.p;

		if(p == end.source->p)
			return 0;

		char* _p = p;
		p += itemSize;

		if(p == bufEnd)
			p = buf;

		end.p = p;
		return _p;
	}

	virtual void finish()
	{
		ProducerConsumer* lastStage = autoGet ? beginning.source : end.source;

		while(lastStage->p != beginning.p)
			lastStage->newProductsSignal->wait();
	}
};

pgn::Pipeline* pgn::Pipeline::create(int itemSize, int maxNumInFlightProducts, int numStages, pgn::PipelineStage* stages[], bool autoGet)
{
	return debug_new ::Pipeline(itemSize, maxNumInFlightProducts, numStages, stages, autoGet);
}

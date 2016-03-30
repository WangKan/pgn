/*

	һ�Σ�������ҵ������ڡ������͡���Ľ�ɫ����Ӧ�Ĵ��룬���޸�����һ�Σ����Լ�������һЩ�����������
	��������һ����ᵽƾ�մ��춫������Ȥ�����ң���Ϊһ������Ա��������Ҫ�����������ˣ�ֻҪ���Ĵ����߼�
	��ȷ������������Ĺ������ͻṤ���úܺã�һ�ж���ô����

	һ�С����������˸�ĸ������������

																	�������� ��DOOM����¼��

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
	int entrySize;
	char *buf, *bufEnd;
	ProducerConsumer* source;
	volatile char* p;
	pgn::Signal* newProductsSignal;

	void init(int entrySize, char* buf, char* bufEnd, ProducerConsumer* source)
	{
		this->entrySize = entrySize;
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
				_p += entrySize;

				if (_p == bufEnd)
					_p = buf;

				p = _p;
				newProductsSignal->raise();
			}
		}

		callbacks->onEnd();
	}

	void init(int entrySize, char* buf, char* bufEnd, ProducerConsumer* source, pgn::PipelineStage* callbacks)
	{
		ProducerConsumer::init(entrySize, buf, bufEnd, source);
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
	int entrySize;
	int sizeBuf;
	char *buf, *bufEnd;
	bool autoGet;
	ProducerConsumer beginning;
	ProducerConsumer end;
	std::vector<Stage> stages;

	Pipeline(int entrySize, int maxNumInFlightProducts, int numStages, pgn::PipelineStage* stageCallbacks[], bool autoGet)
	{
		this->entrySize = entrySize;
		sizeBuf = entrySize * (maxNumInFlightProducts + 1);
		buf = debug_new char[sizeBuf];
		bufEnd = buf + sizeBuf;
		this->autoGet = autoGet;

		ProducerConsumer* producer;

		beginning.init(entrySize, buf, bufEnd, 0);
		producer = &beginning;

		stages.resize(numStages);
		for(int i = 0; i < numStages; i++)
		{
			stages[i].init(entrySize, buf, bufEnd, producer, stageCallbacks[i]);
			producer = &stages[i];
		}

		if(!autoGet)
		{
			end.init(entrySize, buf, bufEnd, producer);
			producer = &end;
		}

		beginning.source = producer;
	}

	virtual void dispose()
	{
		for(int i = (int)stages.size() - 1; i >= 0; i--) // ��Ȼû��һ�������ŵ�size_t
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

	virtual bool put(void* entry)
	{
		char* p = (char*)beginning.p;

		ptrdiff_t d = beginning.source->p - p;

		if (d < 0)
			d += sizeBuf;

		if (d == entrySize)
			return false;

		memcpy(p, entry, entrySize);

		p += entrySize;

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
		p += entrySize;

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

pgn::Pipeline* pgn::Pipeline::create(int entrySize, int maxNumInFlightProducts, int numStages, pgn::PipelineStage* stages[], bool autoGet)
{
	return debug_new ::Pipeline(entrySize, maxNumInFlightProducts, numStages, stages, autoGet);
}

#include <PGN/Common/SharedInterface.h>

class SharedInterfaceImpl : public virtual pgn::SharedInterface
{
	int refCount;

public:
	SharedInterfaceImpl()
	{
		refCount = 1;
	}

	virtual void retain()
	{
		refCount++;
	}

	virtual void release()
	{
		if (--refCount == 0)
			destroy();
	}
};

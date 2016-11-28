namespace pgn {

template<class T, int capacity>
class Queue
{
public:
	T a[capacity];
	int size;
	int begin, end;

	Queue()
	{
		size = 0;
		begin = 0;
		end = 0;
	}

	void push(T& e)
	{
		a[end++] = e;
		end %= capacity;
		size++;
	}

	void pop()
	{
		begin++;
		begin %= capacity;
		size--;
	}

	T& front()
	{
		return a[begin];
	}

	T& back()
	{
		return a[begin];
	}

	bool empty()
	{
		return size == 0;
	}

	bool full()
	{
		return size == capacity;
	}
};

}

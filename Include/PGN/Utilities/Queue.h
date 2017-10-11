#include <vector>

namespace pgn {

template<class T>
class Queue
{
public:
	std::vector<T> a;
	int size;
	int begin, end;

	Queue()
	{
		size = 0;
		begin = 0;
		end = 0;
	}

	Queue(int capacity)
	{
		a.resize(capacity);
		size = 0;
		begin = 0;
		end = 0;
	}

	void push(T& e)
	{
		if (a.size() == size)
		{
			a.emplace_back();

			if (begin != 0)
			{
				for (size_t i = a.size() - 1; i > begin; i--)
					a[i] = a[i - 1];

				begin++;
			}
		}

		end++;
		end %= a.size();
		a[end] = e;
		size++;
	}

	void pop()
	{
		begin++;
		begin %= a.size();
		size--;
	}

	T& front()
	{
		return a[begin];
	}

	T& back()
	{
		return a[end];
	}

	bool empty()
	{
		return size == 0;
	}

	bool full()
	{
		return size == a.size();
	}
};

}

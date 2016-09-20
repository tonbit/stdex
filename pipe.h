#ifndef STDEX_PIPE_H_
#define STDEX_PIPE_H_

#include "stdcc.h"
#include "queue.h"
#include <thread>
namespace stdex {

template<class Task>
class Pipe
{
public:
	Pipe() : _alive(true), _thread(entry, this)
	{
	}

	~Pipe()
	{
		_alive = false;
		_thread.join();
	}

	inline void push(const Task &task)
	{
		_queue.push(task);
	}

	inline void push(Task &&task)
	{
		_queue.push(std::move(task));
	}

private:
	bool _alive;
	Queue<Task> _queue;
	std::thread _thread;

	static void entry(Pipe *pipe)
	{
		while (pipe->_alive)
		{
			Task task;

			if (!pipe->_queue.wait(task, 100))
				task();
		}
	}
};

}
#endif //STDEX_PIPE_H_

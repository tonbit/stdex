#ifndef STDEX_QUEUE_H_
#define STDEX_QUEUE_H_

#include "stdex/stdcc.h"
#include <list>
#include <mutex>
#include <condition_variable>
namespace stdex {

template<class T>
class Queue
{
public:
	inline void push(const T &obj)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _list.push_back(obj);
        _cond.notify_one();
    }

	inline void push(T &&obj)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _list.push_back(std::move(obj));
        _cond.notify_one();
    }

	inline void pop(T &obj)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [&](){return !_list.empty();});
        obj = std::move(_list.front());
        _list.pop_front();
    }

	inline int wait(T &obj, int msec)
    {
        std::unique_lock<std::mutex> lock(_mutex);

        if (!_cond.wait_for(lock, std::chrono::milliseconds(msec), [&](){return !_list.empty();}))
            return 1;

        obj = std::move(_list.front());
        _list.pop_front();
        return 0;
    }

	inline void roll_back(const T &obj)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _list.push_front(obj);
        _cond.notify_one();
    }

	inline void roll_back(T &&obj)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _list.push_front(std::move(obj));
        _cond.notify_one();
    }

    inline size_t size()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _list.size();
    }

    inline bool empty()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _list.empty();
    }

    inline void clear()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _list.clear();
    }

private:
	std::list<T> _list;
    std::mutex _mutex;
	std::condition_variable _cond;
};

}
#endif //STDEX_QUEUE_H_

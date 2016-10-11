#ifndef STDEX_QUEUE_H_
#define STDEX_QUEUE_H_

#include "stdex/stdcc.h"
#include "stdex/optional.h"
#include <mutex>
#include <condition_variable>
namespace stdex {

template<typename Type>
class Queue
{
public:
	inline void push(const Type &obj)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _deque.push_back(obj);
        _cond.notify_one();
    }

	inline void push(Type &&obj)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _deque.push_back(std::move(obj));
        _cond.notify_one();
    }

	inline Type pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [&](){return !_deque.empty();});
        Type obj = std::move(_deque.front());
        _deque.pop_front();
        return std::move(obj);
    }

	inline optional<Type> wait(int msec)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_cond.wait_for(lock, std::chrono::milliseconds(msec), [&](){return !_deque.empty();}))
        {
			static optional<Type> nullopt;
            return std::move(nullopt);
		}

        optional<Type> obj = std::move(_deque.front());
        _deque.pop_front();
        return std::move(obj);
    }

    inline void clear()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _deque.clear();
    }

    inline size_t size()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _deque.size();
    }

    inline bool empty()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _deque.empty();
    }

private:
	std::deque<Type> _deque;
    std::mutex _mutex;
	std::condition_variable _cond;
};


#ifndef JOURNAL_SIZE
#define JOURNAL_SIZE (20*1024*1024)
#endif

template<typename Type>
class DurableQueue
{
public:
	DurableQueue(std::function<std::string(const Type &)> serialize,
				std::function<Type(const std::string &)> deserialize,
				bool sync_write = false,
				size_t journal_size = JOURNAL_SIZE)
	{
		_serialize = serialize;
		_deserialize = deserialize;
		_sync_write = sync_write;
		_journal_size = journal_size;
	}

	~DurableQueue()
	{
		close();
	}

	int open(const string &path)
	{
        std::unique_lock<std::mutex> lock(_mutex);
        _journal_path = path;
		recover(); //recover from journal
		_journal.open(path, std::ios::app);
		if (!_journal)
			return 1;
		return 0;
	}

	void close()
	{
        std::unique_lock<std::mutex> lock(_mutex);
		if (_journal.is_open())
			_journal.close();
	}

	inline void push(const Type &obj)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _deque.push_back(obj);
        _journal << '+' << _serialize(obj) << '\n';
		if (_sync_write)
			_journal.flush();
        _cond.notify_one();
    }

	inline void push(Type &&obj)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _deque.push_back(std::move(obj));
        _journal << '+' << _serialize(_deque.back()) << '\n';
		if (_sync_write)
			_journal.flush();
        _cond.notify_one();
    }

	inline Type pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [&](){return !_deque.empty();});

        Type obj = std::move(_deque.front());
        _deque.pop_front();
		_journal << "-\n";
		if (_sync_write)
			_journal.flush();
		if  (_deque.empty() && _journal.tellp() > _journal_size)
			truncate();
        return std::move(obj);
    }

	inline optional<Type> wait(int msec)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_cond.wait_for(lock, std::chrono::milliseconds(msec), [&](){return !_deque.empty();}))
        {
			static optional<Type> nullopt;
        	return std::move(nullopt);
		}

        optional<Type> obj = std::move(_deque.front());
        _deque.pop_front();
		_journal << "-\n";
		if (_sync_write)
			_journal.flush();
		if  (_deque.empty() && _journal.tellp() > _journal_size)
			truncate();
        return std::move(obj);
    }

    inline void clear()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _deque.clear();
		_journal << " \n";
		if (_sync_write)
			_journal.flush();
		if  (_journal.tellp() > _journal_size)
			truncate();
    }

    inline size_t size()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _deque.size();
    }

    inline bool empty()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _deque.empty();
    }

private:
	std::deque<Type> _deque;
    std::mutex _mutex;
	std::condition_variable _cond;
	std::function<std::string(const Type &)> _serialize;
	std::function<Type(const std::string &)> _deserialize;
	bool _sync_write;
	size_t _journal_size;
	string _journal_path;
	std::ofstream _journal;

	int recover()
	{
		std::ifstream journal;

		journal.open(_journal_path, std::ios::in);

		if (!journal)
			return 1;

		char buf[128];
		string line;

		while (!journal.eof())
		{
			journal.getline(buf, sizeof(buf));
			line.append(buf);

			if (journal.rdstate() == std::ios_base::failbit)
			{
				journal.clear();
				continue;
			}

			char opt = line[0];

			if (opt == '+')
			{
				Type tmp = _deserialize(&line[0] + 1);
				_deque.push_back(std::move(tmp));
			}
			else if (opt == '-')
			{
				assert(line.size() == 1);
				_deque.pop_front();
			}
			else if (opt == ' ')
			{
				assert(line.size() == 1);
				_deque.clear();
			}

			line.resize(0);
		}

		journal.close();
		return 0;
	}

	int truncate()
	{
		_journal.close();

		std::ofstream journal;
		journal.open(_journal_path, std::ios::trunc);
		journal.close();

		_journal.open(_journal_path, std::ios::app);
		if (!_journal)
			return 1;
		return 0;
	}
};

}
#endif //STDEX_QUEUE_H_

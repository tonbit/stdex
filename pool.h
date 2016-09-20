#ifndef STDEX_POOL_H_
#define STDEX_POOL_H_

#include "stdex/stdcc.h"
#include "stdex/misc.h"
namespace stdex {

class MemoryPool
{
public:
    inline MemoryPool()
	{
	    _head = NULL;
	}

    inline ~MemoryPool()
	{
		Node *node;

		while (_head)
		{
		    node = _head->next;
		    free(_head);
		    _head = node;
		}
	}

    void *alloc(size_t size)
	{
		Node *node;
		char *ptr;

	    size = align(size);
        assert(size<=0x1000);

        if (_head->left >= size)
        {
            ptr = _head->buf + _head->last;
            _head->last += size;
            _head->left -= size;
            return ptr;
        }

		node = (Node *)malloc(0x10000);
		node->next = _head;
		_head = node;

		ptr = node->buf;
		node->last = size;
		node->left = 0x10000-sizeof(Node)-size;
		return ptr;
	}

private:

    struct Node
    {
        Node *next;
        size_t last;
        size_t left;
        char buf[0];
    };

    Node *_head;
};

template<class T>
class ObjectPool
{
public:

	inline ObjectPool()
	{
	    _head = NULL;
	}

	inline ~ObjectPool()
	{
	}

    inline T *alloc()
	{
	    if (_head)
    		return new ((void *)fetch_node()) T;
    	else
    	    return new (_pool.alloc(sizeof(T))) T;
	}

    inline T *alloc(const T &ref)
	{
	    if (_head)
    		return new ((void *)fetch_node()) T(ref);
    	else
    	    return new (_pool.alloc(sizeof(T))) T(ref);
	}

    inline void free(T *obj)
	{
	    obj->~T(); //destruction
		cache_node((Node *)obj);
	}

	inline void reserve(int num)
	{
	    while (num--)
	        cache_node(_pool.alloc(sizeof(T)));
	}

private:

    struct Node
    {
        Node *next;
    };

	inline void cache_node(Node *node)
	{
	    node->next = _head;
	    _head = node;
	    return;
	}

	inline Node *fetch_node()
	{
	    Node *node = _head;
	    _head = node->next;
	    return node;
	}

    MemoryPool _pool;
    Node *_head;
};

}
#endif //STDEX_POOL_H_

#ifndef INDEXEDBINARYHEAP_HH
#define INDEXEDBINARYHEAP_HH

#ifndef STD_MAP
#include <map>
#define STD_MAP
#endif

#ifndef STDINT_H
#include <stdint.h>
#define STDINT_H
#endif

#ifndef BOOST_UNORDERED_MAP_HPP
#include <boost/unordered_map.hpp>
#define BOOST_UNORDERED_MAP_HPP
#endif

#ifndef STD_VECTOR
#include <vector>
#define STD_VECTOR
#endif

#ifndef BOOST_ASSERT_HPP
#include <boost/assert.hpp>
#define BOOST_ASSERT_HPP
#endif

#ifndef STD_IOSTREAM
#include <iostream>
#define STD_IOSTREAM
#endif

/**
 * A binary min heap of elements T, which have methods:
 *      P T::priority() const
 *      K T::key() const
 * Requires boost::hash<K>.
 * Has support for removing, and up/down-heaping arbitrary elements.
 */
template<typename T, typename K, typename P=uint32_t>
class IndexedBinaryHeap
{
    typedef uint32_t heap_pos;
    typedef boost::unordered_map<K, heap_pos> PosMap;

public:

    void dump(uint64_t k)
    {
        for (uint64_t i = 0; i < mHeap.size(); ++i)
        {
            std::cerr << kmerToString(k, mHeap[i].mNode.value()) << " ";
        }
        std::cerr << '\n';

        for (typename PosMap::const_iterator i = mPos.begin(); i != mPos.end(); ++i)
        {
            std::cerr << kmerToString(k, i->first.value()) << " -> " << i->second << '\n';
        }
    }

    /**
     * The least-valued element.
     */
    const T& top() const
    {
        BOOST_ASSERT(mHeap.size());
        return mHeap.front();
    }

    /**
     * Removes and returns the least-valued element.
     */
    T pop()
    {
        BOOST_ASSERT(mHeap.size());
        T t = top();
        remove(t);
        valid();
        return t;
    }

    /**
     * Inserts an element into the heap.
     */
    void push(const T& t)
    {
        mHeap.push_back(t);
        heap_pos pos = mHeap.size();
        mPos.insert(make_pair(t.key(), pos));
        up(pos);
        valid();
    }

    /**
     * Removes the given element from the heap.
     */
    void remove(const T& t)
    {
        valid();
        BOOST_ASSERT(mHeap.size());
        typename PosMap::iterator itr(mPos.find(t.key()));
        heap_pos p = itr->second;
        if (p == size())
        {
            // Removing the last heap element - don't replace it.
            mHeap.pop_back();
            mPos.erase(itr);
        }
        else
        {
            T u = mHeap.back();
            mPos.erase(itr);
            at(p) = u;
            mHeap.pop_back();
            mPos[u.key()] = p;
            down(p);
        }
        valid();
    }

    /**
     * Move the given element up until the heap condition is restored.
     * This should be called when an element's priority decreases.
     */
    void up(const T& t)
    {
        typename PosMap::const_iterator itr(mPos.find(t.key()));
        BOOST_ASSERT(itr != mPos.end());
        up(itr->second);
        valid();
    }

    /**
     * Move the given element down until the heap condition is restored.
     * This should be called when an element's priority increases.
     */
    void down(const T& t)
    {
        typename PosMap::const_iterator itr(mPos.find(t.key()));
        BOOST_ASSERT(itr != mPos.end());
        down(itr->second);
    }

    heap_pos size() const
    {
        return maxPos();
    }

    T& get(const K& pKey)
    {
        BOOST_ASSERT(mPos.find(pKey) != mPos.end());
        valid();
        return at(mPos[pKey]);
    }

    bool contains(const K& pKey)
    {
        valid();
        return mPos.find(pKey) != mPos.end();
    }

    IndexedBinaryHeap()
        : mHeap(), mPos()
    {
    }

private:

    void valid()
    {
        return;
        BOOST_ASSERT(mHeap.size() == mPos.size());
        for (heap_pos i = 1; i <= size(); ++i)
        {
            T t = at(i);
            typename PosMap::const_iterator itr(mPos.find(t.key()));
            BOOST_ASSERT(itr != mPos.end());
            BOOST_ASSERT(itr->second == i);
        }

        for (typename PosMap::const_iterator i = mPos.begin(); i != mPos.end(); ++i)
        {
            BOOST_ASSERT(i->first == at(i->second).key());
        }
    }

    void up(heap_pos pPos)
    {
        valid();
        heap_pos c = pPos;
        heap_pos p = parent(c);
        while (c != 1 && at(p).priority() > at(c).priority())
        {
            exchange(p, c);
            c = p;
            p = parent(c);
        }
        valid();
    }

    void down(heap_pos pPos)
    {
        valid();
        bool moved;
        heap_pos p = pPos;
        heap_pos cl = leftChild(p);
        heap_pos cr = rightChild(p);
        heap_pos min = p;
        do
        {
            moved = false;
            if (cl <= maxPos() && at(cl).priority() < at(p).priority())
            {
                min = cl;
            }
            if (cr <= maxPos() && at(cr).priority() < at(min).priority())
            {
                min = cr;
            }
            if (min != p)
            {
                exchange(p, min);
                p = min;
                cl = leftChild(p);
                cr = rightChild(p);
                moved = true;
            }
        }
        while (moved);
        valid();
    }

    heap_pos leftChild(heap_pos pPos) const
    {
        return pPos * 2;
    }

    heap_pos rightChild(heap_pos pPos) const
    {
        return pPos * 2 + 1;
    }

    heap_pos parent(heap_pos pPos) const
    {
        return pPos / 2;
    }

    /**
     * Swaps the value at pPos1 with that at pPos2.
     */
    void exchange(heap_pos pPos1, heap_pos pPos2)
    {
        valid();
        T t1(at(pPos1));
        T t2(at(pPos2));
        mPos[t1.key()] = pPos2;
        mPos[t2.key()] = pPos1;
        at(pPos1) = t2;
        at(pPos2) = t1;
        valid();
    }

    T& at(heap_pos pPos)
    {
        return mHeap[pPos - 1];
    }

    heap_pos maxPos() const
    {
        return mHeap.size();
    }

    std::vector<T> mHeap;
    PosMap mPos;
};

#endif // BINARYHEAP_HH
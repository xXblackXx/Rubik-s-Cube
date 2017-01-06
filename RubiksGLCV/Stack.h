#ifndef STACK_H
#define STACK_H

#include "Cubies.h"

#include <memory>

#define BLOCKSIZE_STACK 10000

using namespace std;

struct DataBlock
{
    Cubies* node;
    unsigned char distance;

    DataBlock() {
        node = nullptr ;
    }

    DataBlock(Cubies* cbs, int dist)
    {
        node = cbs;
        distance = dist;
    }
    void clr()
    {
        if ( node != nullptr )
            delete node ;
    }
};

class Stack
{

    struct Block
    {
        struct Block *leftlink;
        DataBlock data[BLOCKSIZE_STACK];
        Block()
        {
            leftlink = nullptr ;
        }
        ~Block()
        {
            delete leftlink ;
        }
    };

    Block* rightblock;
    int rightindex; /* index into last block, points to last element */

public:
    long length;


    Stack()
    {
        length = 0;
        rightblock = nullptr;
    }

    bool push(Cubies* cbs, int dist) // O(1)
    {
        if (rightblock == nullptr) // O(1)
        {
            /* initialize */
            rightblock = new Block(); // O(1)
            rightblock->leftlink = nullptr; // O(1)
            length = 0; // O(1)
            rightindex = -1; // O(1)
        }
        else if (rightindex == BLOCKSIZE_STACK-1) // O(1)
        {
            /* Needs a new block */
            Block* newblock = new Block; // O(1)
            newblock->leftlink = rightblock; // O(1)
            rightblock = newblock; // O(1)
            rightindex = -1; // O(1)
        }
        length++; // O(1)
        rightindex++; // O(1)
        /* Now copy data in */
        rightblock->data[rightindex].node = cbs; // O(1)
        rightblock->data[rightindex].distance = dist; // O(1)

        return true; // O(1)
    }

    bool empty()
    {
        if (length == 0) return true;
        return false;
    }

    DataBlock* top()
    {
        return &rightblock->data[rightindex];
    }
    bool release_pop()
    {

    }
    Cubies* pop()
    {
        /* Just erase the last element */
        assert(rightindex != -1) ;
        length--;
        Cubies *current = rightblock->data[rightindex].node ;
        if (rightindex == 0 && length > 0)
        {
            /* Freeing an entire block */
            Block* oldblock = rightblock;
            rightblock = oldblock->leftlink;
            delete oldblock;
            rightindex = BLOCKSIZE_STACK - 1;
        }
        else
        {
            rightindex--;
        }
        return current;
    }
    bool pop_clr()
    {
        /* Just erase the last element */
        assert(rightindex != -1) ;
        length--;
        if (rightindex == 0)
        {
            rightblock->data[rightindex].clr() ;
            /* Freeing an entire block */
            Block* oldblock = rightblock;
            rightblock = oldblock->leftlink;
            delete oldblock;
            rightindex = BLOCKSIZE_STACK - 1;
        }
        else
        {
            rightblock->data[rightindex].clr() ;
            rightindex--;
        }
        return true;
    }
    int size()
    {
        return length;
    }

};

#endif // STACK_H

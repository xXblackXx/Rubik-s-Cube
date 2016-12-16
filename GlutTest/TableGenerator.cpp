#include "TableGenerator.h"

TableGenerator::TableGenerator()
{
    //ctor
}

TableGenerator::~TableGenerator()
{
    //dtor
}

void TableGenerator::EdgeTableGenerator(Byte* EDGE_TABLE, bool EdgeSet)
{
    int hash;
    int depth = 0;
    int nodesCount = 0;
    int popCount = 0;

    Stack _stack;

    Cube c = Cube(solvedCube);
    Cubies GoalCube = Cubies::Copy(c.toCubiesFromSides());

    int count = 0;

    Byte* instack = new Byte[21288960];

    memset(EDGE_TABLE, 0, 21288960);


    DataBlock current;
    Cubies* NextStates[20];

    while (nodesCount < 42577920)
    {
        /* if stack is empty, go up a level */
        if (_stack.length == 0)
        {
            _stack.push(&GoalCube, 0);
            depth++;
            /* clear out instack table */
            memset(instack, 255, 21288960);
        }

        /* Pop the first item off, put it in current */
        popCount++;
        current = _stack.top();
        _stack.pop();

        /* Print out status every 2^18 pops  (approx every 200k)*/
        if ((popCount & 077777) == 077777)
            fprintf(stderr, "\r%d/42577920 hashed, on level:%d/10, total traversed:%d", nodesCount, depth, popCount);

        //if (popCount > 1000000) return;

        current.node->GenerateNextStates(NextStates);

        for (int i = 0; i < current.node->statesCount; i++)
        {
            if (EdgeSet) hash = NextStates[i]->GetEdge1Hash();
            else hash = NextStates[i]->GetEdge2Hash();

            if (hash&1 ? \
                    ((instack[(hash-1)/2] >> 4) <= (current.distance+1)) : \
                    ((instack[hash/2] & 15) <= (current.distance+1))) {
                continue;
            }
            /* add to instack */
            if (hash&1)
            {
                instack[(hash-1)/2] &= 15;
                instack[(hash-1)/2] |= (current.distance+1) << 4;
            }
            else
            {
                instack[hash/2] &= 15<<4;
                instack[hash/2] |= (current.distance+1);
            }

            if (current.distance + 1 == depth)
            {
                /*
                 * if item is at our current target depth, add it to hash table
                 */
                if (hash & 1)
                {
                    if (!(EDGE_TABLE[(hash-1)/2] >> 4))
                    {
                        EDGE_TABLE[(hash-1)/2] |= (current.distance+1) << 4;
                        nodesCount++;
                    }
                    else
                    {
                        /* A duplicate, skip */
                    }
                }
                else
                {
                    if (!(EDGE_TABLE[hash/2]&15))
                    {
                        EDGE_TABLE[hash/2] |= current.distance + 1;
                        nodesCount++;
                    }
                    else
                    {
                        /* a duplicate */
                    }
                }
            }
            else
            {
                /* Add to real stack */
                _stack.push(NextStates[i], current.distance+1);
            }
        }
        //current.node->statesCount = 0;
    }


    if (EdgeSet) hash = GoalCube.GetEdge1Hash();
    else hash = GoalCube.GetEdge2Hash();

    if (hash & 1) {
        /* zero out upper bits */
        EDGE_TABLE[(hash-1)/2] &= 15;
    } else {
        /* zero out lower bits */
        EDGE_TABLE[hash/2] &= (15<<4);
    }

}

void TableGenerator::CornerTableGenerator(Byte* CORNER_TABLE)
{
    int f;
    int hash;
    int depth = -1;
    int nodesCount = 0;
    int popCount = 0;

    Stack _stack;

    Cube c = Cube(solvedCube);
    Cubies GoalCube = Cubies::Copy(c.toCubiesFromSides());

    int count = 0;

    Byte* instack = new Byte[44089920];
    memset(CORNER_TABLE, 0, 44089920);
    while (count < 88179840)
    {
        if (_stack.size() == 0)
        {
            _stack.push(&GoalCube, 0);
            depth++;
            memset(instack, 255, 44089920);
        }

        popCount++;
        DataBlock current = _stack.top();
        _stack.pop();

        /* Print out status every 2^18 pops  (approx every 200k)*/
        if ((popCount & 077777) == 077777)
        {
            fprintf(stderr, "\r%d/88179840 hashed, on level:%d/11, total traversed:%d %d%% ", count, depth, popCount, popCount / 5788130);
        }

        if (current.distance == depth)
        {
            hash = current.node->GetCornerHash();
            if (hash & 1)
            {
                if (!(CORNER_TABLE[(hash - 1) / 2] >> 4))
                {
                    CORNER_TABLE[(hash - 1) / 2] |= current.distance << 4;
                    count++;
                }
                else
                {
                    /* A duplicate, skip */
                }
            }
            else
            {
                if (!(CORNER_TABLE[hash / 2] & 15))
                {
                    CORNER_TABLE[hash / 2] |= current.distance;
                    count++;
                }
                else
                {
                    /* a duplicate */
                }
            }
        }
        else
        {
            /*
            * not at depth yet, Generate Next States, applying heuristics pruning
            */

            Cubies* NextStates[20]; //= new shared_ptr<Cubies>[20];

            current.node->GenerateNextStates(NextStates);


            for (int i = 0; i < current.node->statesCount; i++)
            {
                Cubies* cbs = NextStates[i];

                hash = cbs->GetCornerHash();

                if (hash & 1 ? \
                        ((instack[(hash - 1) / 2] >> 4) <= (current.distance + 1)) : \
                        ((instack[hash / 2] & 15) <= (current.distance + 1)))
                {
                    continue;
                }
                /* add to instack */
                if (hash & 1)
                {
                    instack[(hash - 1) / 2] &= 15;
                    instack[(hash - 1) / 2] |= (current.distance + 1) << 4;
                }
                else
                {
                    instack[hash / 2] &= 15 << 4;
                    instack[hash / 2] |= (current.distance + 1);
                }

                /* Add to real stack */
                _stack.push(cbs, current.distance + 1);
            }
            current.node->statesCount = 0;
        }
    }
}

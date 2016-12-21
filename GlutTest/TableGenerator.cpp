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
    int hashCube;
    int depth = 0;
    int nodescountNodes = 0;
    int popcountNodes = 0;

    Stack _stack;

    Cube c = Cube(solvedCube);
    Cubies GoalCube = Cubies::Copy(c.toCubiesFromSides());

    int countNodes = 0;


    memset(EDGE_TABLE, 0, 21288960);

    DataBlock current;
    Cubies* NextStates[20];
    Cubies* currentNode ;

    while (nodescountNodes < 42577920) //42577920
    {
        /* if stack is empty, go up a level */
        if (_stack.length == 0)
        {
            _stack.push(new Cubies(GoalCube), 0);
            depth++;
        }

        /* Pop the first item off, put it in current */
        popcountNodes++;
        current = _stack.top();
        currentNode = _stack.pop();

        /* Print out status every 2^18 pops  (approx every 200k)*/
        if ((popcountNodes & 0777777) == 0777777)
            fprintf(stderr, "\r%d/42577920 hashed, on level:%d/10, total traversed:%d", nodescountNodes, depth, popcountNodes);


        currentNode->GenerateNextStates(NextStates);

        for (int i = 0; i < currentNode->statesCount; i++)
        {
            if (EdgeSet) hashCube = NextStates[i]->GetEdge1Hash();
            else hashCube = NextStates[i]->GetEdge2Hash();

            if (current.distance + 1 == depth)
            {
                /*
                 * if item is at our current target depth, add it to hashCube table
                 */
                if (hashCube & 1)
                {
                    if (!(EDGE_TABLE[(hashCube-1)/2] >> 4))
                    {
                        EDGE_TABLE[(hashCube-1)/2] |= (current.distance+1) << 4;
                        nodescountNodes++;
                    }
                    else
                    {
                        /* A duplicate, skip */
                    }
                }
                else
                {
                    if (!(EDGE_TABLE[hashCube/2]&15))
                    {
                        EDGE_TABLE[hashCube/2] |= current.distance + 1;
                        nodescountNodes++;
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
                Cubies* cbs = new Cubies(*NextStates[i]);

                _stack.push(cbs, current.distance+1);
            }
        }

        for ( int i = 0 ; i < currentNode->statesCount ; i++ )
            delete NextStates[i] ;
        delete currentNode ;
        delete []NextStates ;


    }

    if (EdgeSet) hashCube = GoalCube.GetEdge1Hash();
    else hashCube = GoalCube.GetEdge2Hash();

    if (hashCube & 1) {
        /* zero out upper bits */
        EDGE_TABLE[(hashCube-1)/2] &= 15;
    } else {
        /* zero out lower bits */
        EDGE_TABLE[hashCube/2] &= (15<<4);
    }

}

void TableGenerator::CornerTableGenerator(Byte* CORNER_TABLE)
{
    int f;
    int hashCube;
    int depth = -1;
    int nodescountNodes = 0;
    int popcountNodes = 0;
    Stack _stack;

    Cube c = Cube(solvedCube);
    Cubies GoalCube = Cubies::Copy(c.toCubiesFromSides());

    int countNodes = 0;

    Byte* instack = new Byte[44089920];
    memset(CORNER_TABLE, 0, 44089920);
    Cubies * currentNode ;
    DataBlock current ;
    while (countNodes < 88179840)
    {
        if (_stack.size() == 0)
        {
            _stack.push(&GoalCube, 0);
            depth++;
            memset(instack, 255, 44089920);
        }

        popcountNodes++;
        current = _stack.top();
        currentNode = _stack.pop();

        /* Print out status every 2^18 pops  (approx every 200k)*/
        if ((popcountNodes & 0777777) == 0777777)
        {
            fprintf(stderr, "\r%d/88179840 hashCubeed, on level:%d/11, total traversed:%d %d%% ", countNodes, depth, popcountNodes, popcountNodes / 5788130);
        }

        if (current.distance == depth)
        {
            hashCube = currentNode->GetCornerHash();
            if (hashCube & 1)
            {
                if (!(CORNER_TABLE[(hashCube - 1) / 2] >> 4))
                {
                    CORNER_TABLE[(hashCube - 1) / 2] |= current.distance << 4;
                    countNodes++;
                }
                else
                {
                    /* A duplicate, skip */
                }
            }
            else
            {
                if (!(CORNER_TABLE[hashCube / 2] & 15))
                {
                    CORNER_TABLE[hashCube / 2] |= current.distance;
                    countNodes++;
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

            currentNode->GenerateNextStates(NextStates);


            for (int i = 0; i < currentNode->statesCount; i++)
            {

                hashCube = NextStates[i]->GetCornerHash();

                if (hashCube & 1 ? \
                        ((instack[(hashCube - 1) / 2] >> 4) <= (current.distance + 1)) : \
                        ((instack[hashCube / 2] & 15) <= (current.distance + 1)))
                {
                    continue;
                }
                /* add to instack */
                if (hashCube & 1)
                {
                    instack[(hashCube - 1) / 2] &= 15;
                    instack[(hashCube - 1) / 2] |= (current.distance + 1) << 4;
                }
                else
                {
                    instack[hashCube / 2] &= 15 << 4;
                    instack[hashCube / 2] |= (current.distance + 1);
                }

                Cubies* cbs = new Cubies(*NextStates[i]);
                /* Add to real stack */
                _stack.push(cbs, current.distance + 1);
            }
            for ( int i = 0 ; i < currentNode->statesCount ; i++ )
                delete NextStates[i] ;
            delete []NextStates ;
        }

        delete currentNode ;

    }
}

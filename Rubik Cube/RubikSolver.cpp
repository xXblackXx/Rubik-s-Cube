#include "RubikSolver.h"

RubikSolver::RubikSolver()
{
    //ctor
}

RubikSolver::~RubikSolver()
{
    //dtor
}

RubikSolver::RubikSolver(Cubies* cbs)
{
	goalState = Cube(solvedCube).toCubiesFromSides();
	initialState = cbs;
}


bool RubikSolver::Solve()
{
	int f;
	int depth = -1;
	int nodesCount = 0;
	int popCount = 0;
	Stack _stack;

    // Array to save solution.
    short path[30];
    path[0] = -1;

    DataBlock current;
    Cubies* NextStates[20];
    Cubies initi = *Cubies::Copy(initialState);
    Cubies *deleted ;
    Cubies *cbs ;
    int dist ;
    int statesCount ;
    int index , index2 ;
    while (1)
	{
		if (_stack.length == 0)
		{
			if (Solved)
                return true;
            initialState = new Cubies(initi) ;
			_stack.push(initialState, 0);
			depth++;
			printf("Searching depth %d\n", depth);
		}

    	popCount++;
		current = _stack.top();
        deleted = _stack.pop() ;
        dist = current.distance ;
		/* update the path array */
        if (dist > 0) {
            path[dist - 1] = deleted->lastOp;
            path[dist] = -1;
        }
		if (dist == depth)
		{
			/*
			*  if the cube is at current depth goal, check if it's solved.
			*/
			if (deleted->isSolved())
			{
				Solved = true;
				actionLog = new short[30];
                index = 0;
				while (path[index] != -1)
                {
                    actionLog[index] = path[index];
                    index++;
                }
                actionLog[index] = -1;
                while (_stack.size())
                    _stack.pop_clr();
				//actionLog = current.node->Solution;
                delete deleted;

				break;
			}
		}
		else
		{
			/*
			* not at depth yet, Generate Next States, applying heuristics pruning
			*/

			deleted->GenerateNextStates(NextStates,depth-dist-1);
            statesCount = deleted->statesCount ;

            for (index = 0; index < statesCount; index++)
            {
                for (index2 = index + 1; index2 < statesCount; index2++)
                    if (NextStates[index]->Heuristic() < NextStates[index2]->Heuristic())
                    {
                        cbs = NextStates[index] ;
                        NextStates[index] = NextStates[index2] ;
                        NextStates[index2] = cbs ;
                    }

				_stack.push(NextStates[index], dist + 1);
            }

    	}
	    delete deleted;

    }

    return false;
}

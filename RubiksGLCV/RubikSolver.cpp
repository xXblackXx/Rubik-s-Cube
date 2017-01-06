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
	int f; // O(1)
	int depth = -1; // O(1)
	int nodesCount = 0; // O(1)
	int popCount = 0; // O(1)
	Stack _stack; // O(1)

    // Array to save solution.
    short path[30]; // O(1)
    path[0] = -1; // O(1)

    DataBlock* current; // O(1)
    Cubies* NextStates[20]; // O(1)
    Cubies initi = *Cubies::Copy(initialState); // O(1)
    Cubies *deleted ; // O(1)
    Cubies *cbs ; // O(1)
    int dist ; // O(1)
    int statesCount ; // O(1)
    int index , index2 ; // O(1)


    // b -> branching factor (average of 13).
    // d -> depth (up to 20).
    // e -> expected value of heuristic function.
    // n -> number of states of the entire problem space at given depth.
    // m -> space of patterns table at given depth.
    // O[b^(d-e)] ~ O(n/m)
    while (1)
	{
		if (_stack.length == 0) // O(1)
		{
			if (Solved) // O(1)
                return true; // O(1)
            initialState = new Cubies(initi); // O(1)
			_stack.push(initialState, 0); // O(1)
			depth++; // O(1)
			printf("Searching depth %d\n", depth); // O(1)
		}

    	popCount++; // O(1)
		current = _stack.top(); // O(1)
        deleted = _stack.pop(); // O(1)
        dist = current->distance; // O(1)
		/* update the path array */
        if (dist > 0) { // O(1)
            path[dist - 1] = deleted->lastOp; // O(1)
            path[dist] = -1; // O(1)
        }


        // This implements DFS using a stack instead of internal function stack for performance sake.
        // total of O[b^(d - e)]
        //only executed once when solution is found amortized
        // O(D) where D -> Depth (length of solution)
        // Amortized complexity of O(1)
		if (dist == depth)
		{
			/*
			*  if the cube is at current depth goal, check if it's solved.
			*/

			// O(D) -> Depth( length of solution)
            // isSolved -> O(1)
			if (deleted->isSolved())
			{
				Solved = true; // O(1)
				actionLog = new short[30]; // O(1)
                index = 0; // O(1)
				while (path[index] != -1) // O(D) where D -> depth (length of solution)
                {
                    actionLog[index] = path[index]; // O(1)
                    index++; // O(1)
                }
                actionLog[index] = -1; // O(1)
                while (_stack.size()) // O(N) where
                    _stack.pop_clr(); // O(1)

				//actionLog = current.node->Solution;
                delete deleted; // O(1)

				break; // O(1)
			}
		}
		else
		{
			/*
			* not at depth yet, Generate Next States, applying heuristics pruning
			*/
			// O(1) -> generate maximum of 18 nodes
			deleted->GenerateNextStates(NextStates,depth-dist-1);
            statesCount = deleted->statesCount ; //O(1)

            for (index = 0; index < statesCount; index++) // O(N^2) where N -> Number of generated states
            {
                for (index2 = index + 1; index2 < statesCount; index2++) // O(N)
                    if (NextStates[index]->Heuristic() < NextStates[index2]->Heuristic()) // O(1)
                    {
                        cbs = NextStates[index] ; // O(1)
                        NextStates[index] = NextStates[index2]; // O(1)
                        NextStates[index2] = cbs ; // O(1)
                    }

				_stack.push(NextStates[index], dist + 1); // O(1)
            }

    	}
	    delete deleted; // O(1)
    }
    return false;
}

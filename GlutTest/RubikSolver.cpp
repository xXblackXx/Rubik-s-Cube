#include "RubikSolver.h"

RubikSolver::RubikSolver()
{
    //ctor
}

RubikSolver::~RubikSolver()
{
    //dtor
}

RubikSolver::RubikSolver(shared_ptr<Cubies> cbs)
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
    shared_ptr<Cubies> NextStates[20];

	while (1)
	{
		if (_stack.length == 0)
		{
			if (Solved)
                return true;

			_stack.push(initialState, 0);
			depth++;
			printf("Searching depth %d, Nodes traversed: %ld\n", depth, popCount);
		}

		popCount++;
		current = _stack.top();
		_stack.pop();

		/* update the path array */
        if (current.distance > 0) {
            path[current.distance - 1] = current.node->lastOp;
            path[current.distance] = -1;
        }

		if (current.distance == depth)
		{
			/*
			*  if the cube is at current depth goal, check if it's solved.
			*/
			if (current.node->isSolved())
			{
				Solved = true;
				actionLog = new short[30];
				int i = 0;
				while (path[i] != -1)
                {
                    actionLog[i] = path[i];
                    i++;
                }
                actionLog[i] = -1;

				//actionLog = current.node->Solution;
				break;
			}
		}
		else
		{
			/*
			* not at depth yet, Generate Next States, applying heuristics pruning
			*/

			current.node->GenerateNextStates(NextStates);

            for (int i = 0; i < current.node->statesCount; i++)
                for (int j = i + 1; j < current.node->statesCount; j++)
                    if (NextStates[i]->Heuristic() > NextStates[j]->Heuristic()) swap(NextStates[i], NextStates[j]);

			for (int i = 0; i < current.node->statesCount; i++)
			{
				shared_ptr<Cubies> cbs = NextStates[i];

				f = current.distance + cbs->Heuristic() + 1;

				if (f > depth)
                    continue;

				_stack.push(cbs, current.distance + 1);
			}
		}
	}
	return false;
}

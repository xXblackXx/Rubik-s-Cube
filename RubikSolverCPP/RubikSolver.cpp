#include "stdafx.h"
#include "RubikSolver.h"

RubikSolver::RubikSolver()
{
}

RubikSolver::RubikSolver(shared_ptr<Cubies> cbs)
{
	goalState = Cube::Cube(solvedCube).toCubiesFromSides();
	initialState = cbs;
}

RubikSolver::~RubikSolver()
{
}

int RubikSolver::Search(shared_ptr<Cubies> node, int g, int threshold, int depth)
{
	node->GenerateNextStates();

	NodesCount++;
	maxDepth = max(maxDepth, depth);

	int f = g + node->Heuristic();

	if (f > threshold)
		return f;

	if (*node == goalState)
	{
		solvedNode = node;
		return FOUND;
	}

	int min = INT_MAX;

	while (node->HasNext())
	{
		shared_ptr<Cubies> tempnode = node->NextState();

		int temp = Search(tempnode, g + 1, threshold, depth + 1);

		if (temp == FOUND)
			return FOUND;
		if (temp < min)
			min = temp;
	}
	node->resetNext();
	return min;
}

bool RubikSolver::Solve()
{
	int threshold = initialState->Heuristic();

	while (true)
	{
		int temp = Search(initialState, 0, threshold, 0);

		if (temp == FOUND)
		{
			Solved = true;
			return Solved;
		}

		if (temp == INT_MAX)
			return Solved;

		threshold = temp;
		totalNodesCount += NodesCount;
		NodesCount = 0;
	}
}

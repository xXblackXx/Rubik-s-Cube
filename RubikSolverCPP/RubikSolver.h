#pragma once
#include "stdafx.h"
#include "Cube.h"
#include "Cubies.h"
#include "Utility.h"

#include <vector>
#include <math.h>
#include <algorithm>
#include <memory>

class RubikSolver
{
private:
	int FOUND = 0;
	int maxDepth = 0;

	long long  NodesCount = 0;

	Cubies goalState;
	
	shared_ptr<Cubies> solvedNode;

	int Search(shared_ptr<Cubies> node, int g, int threshold, int depth);
public:
	RubikSolver();
	RubikSolver(shared_ptr<Cubies> cbs);
	~RubikSolver();

	bool Solved = false;
	long long totalNodesCount = 0;
	shared_ptr<Cubies> initialState;

	vector<string> actionLog;

	bool Solve();
};


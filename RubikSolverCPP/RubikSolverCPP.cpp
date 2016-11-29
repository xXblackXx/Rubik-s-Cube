// RubikSolverCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Utility.h"
#include "Cube.h"
#include "RubikSolver.h"

#include <memory>

int main()
{
	//string movs[] = { "D2", "F'", "D'", "U'", "R", "F", "D", "B", "R2", "D'", "B2", "L'", "D'", "F2", "D2", "L2", "U2", "R'", "U'" };
	string movs[] = { "D2", "F'", "D'" };
	shared_ptr<Cubies> cbs = Cubies::Copy(Cube::CubeFromMovesList(movs, 3));
	
	RubikSolver solver(cbs);
	
	//solver.Solve();

    return 0;
}


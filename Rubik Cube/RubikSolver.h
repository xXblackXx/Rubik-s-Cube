#pragma once
#ifndef RUBIKSOLVER_H
#define RUBIKSOLVER_H

#include "Cube.h"
#include "Cubies.h"
#include "Utility.h"
#include "Stack.h"

#include <vector>
#include <stack>
#include <math.h>
#include <algorithm>
#include <memory>

using namespace std;

class RubikSolver
{
public:
    RubikSolver();
    RubikSolver(Cubies* cbs);
    virtual ~RubikSolver();

    bool Solved = false;
    Cubies* initialState;

    short* actionLog;

    bool Solve();
protected:
private:
    Cubies goalState;

	shared_ptr<Cubies> solvedNode;
};

#endif // RUBIKSOLVER_H

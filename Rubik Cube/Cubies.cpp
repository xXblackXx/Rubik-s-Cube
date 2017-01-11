#include "Cubies.h"

Cubies::~Cubies()
{
    //dtor
}

void Cubies::SetPatternTables(Byte* CORNER_TABLE, Byte* EDGE1_TABLE, Byte* EDGE2_TABLE)
{
    PtrCornerTable = CORNER_TABLE;
    PtrEdge1Table = EDGE1_TABLE;
    PtrEdge2Table = EDGE2_TABLE;
}

Cubies::Cubies()
{
    //assume is solved
    //CurrentStage = 0;
    for(short i = 0; i < 20; i++)
    {
        positions[i] = i;
        orientations[i] = 0;
    }
    HeuristicValue = -1;
    lastOp = -1;
    //NextStates = new shared_ptr<Cubies>[18];
}

// enum moves
// {
// 	F = 0,
// 	B,
// 	R,
// 	L,
// 	U,
// 	D
// };


string Cubies::cornerWithOrientation(string c, short orient)
{
    string res = "";
    if(orient == 1)
    {
        res += (c[2] + c[0] + c[1]);
    }
    else
    {
        res += (c[1] + c[2] + c[0]);
    }
    return res;
}
/*
string Cubies::toStringOfFaces()
{
    string cube = "";
    string tmp;
    for(int i = 0; i < 12; i++)
    {
        tmp = fromEdgeToString(positions[i]);
        if(orientations[i] == 1)
            swap(tmp[0], tmp[1]);
        cube += tmp;
    }
    for(int i = 12; i < 20; i++)
    {
        tmp = fromCornerToString(positions[i]);
        if(orientations != 0)
            tmp = this->cornerWithOrientation(tmp, orientations[i]);
        cube += tmp;
    }
    return cube;
}
*/

void Cubies::clockCycle(moves m)
{

    // edges

    short tmpPos = positions[moveTable[m][3]];
    short tmpOr = orientations[moveTable[m][3]];

    for(int i = 3; i > 0; i-- )
    {
        positions[moveTable[m][i]] = positions[moveTable[m][i - 1]];
        orientations[moveTable[m][i]] = orientations[moveTable[m][i - 1]];
    }
    positions[moveTable[m][0]] = tmpPos;
    orientations[moveTable[m][0]] = tmpOr;

    // F, B affects edges orientation (if 0 (correct) -> 1 (incorrect))

    if(m < 2)
    {
        for(int i = 0; i < 4; i++)
        {
            orientations[moveTable[m][i]] = (orientations[moveTable[m][i]] == 1)? 0 : 1;
        }
    }


    // corners

    tmpPos = positions[moveTable[m][7]];
    tmpOr = orientations[moveTable[m][7]];

    for(int i = 7; i > 4; i-- )
    {
        positions[moveTable[m][i]] = positions[moveTable[m][i - 1]];
        orientations[moveTable[m][i]] = orientations[moveTable[m][i - 1]];
    }
    positions[moveTable[m][4]] = tmpPos;
    orientations[moveTable[m][4]] = tmpOr;


    // F, B, R, L affects corner orientation:  0 -> correct, 1 -> cloclwise, 2 -> counterclockwise
    //	 ___ ___ ___
    //	|cw	|   |ccw|
    //	|___|___|___|
    //	|	|   |   |
    //	|___|___|___|
    //	|ccw|   | cw|
    //	|___|___|___|

    if(m < 4)
    {
        // ccw

        orientations[moveTable[m][5]] = (orientations[moveTable[m][5]] + 2) % 3;
        orientations[moveTable[m][7]] = (orientations[moveTable[m][7]] + 2) % 3;

        // cw

        orientations[moveTable[m][4]] = (orientations[moveTable[m][4]] + 1) % 3;
        orientations[moveTable[m][6]] = (orientations[moveTable[m][6]] + 1) % 3;
    }
}


void Cubies::counterCycle(moves m)
{

    // edges

    short tmpPos = positions[moveTable[m][0]];
    short tmpOr = orientations[moveTable[m][0]];

    for(int i = 0; i < 3; i++ )
    {
        positions[moveTable[m][i]] = positions[moveTable[m][i + 1]];
        orientations[moveTable[m][i]] = orientations[moveTable[m][i + 1]];
    }
    positions[moveTable[m][3]] = tmpPos;
    orientations[moveTable[m][3]] = tmpOr;

    if(m < 2)
        for(int i = 0; i < 4; i++)
        {
            orientations[moveTable[m][i]] = (orientations[moveTable[m][i]] == 1)? 0 : 1;
        }


    // corners

    tmpPos = positions[moveTable[m][4]];
    tmpOr = orientations[moveTable[m][4]];

    for(int i = 4; i < 7; i++ )
    {
        positions[moveTable[m][i]] = positions[moveTable[m][i + 1]];
        orientations[moveTable[m][i]] = orientations[moveTable[m][i + 1]];
    }
    positions[moveTable[m][7]] = tmpPos;
    orientations[moveTable[m][7]] = tmpOr;

    //	 ___ ___ ___
    //	|ccw|   |cw |
    //	|___|___|___|
    //	|	|   |   |
    //	|___|___|___|
    //	|cw |   |ccw|
    //	|___|___|___|


    if(m < 4)
    {
        // cw

        orientations[moveTable[m][5]] = (orientations[moveTable[m][5]] - 1) % 3;
        orientations[moveTable[m][7]] = (orientations[moveTable[m][7]] - 1) % 3;

        if (orientations[moveTable[m][5]] < 0) orientations[moveTable[m][5]] += 3;
        if (orientations[moveTable[m][7]] < 0) orientations[moveTable[m][7]] += 3;

        // ccw

        orientations[moveTable[m][4]] = (orientations[moveTable[m][4]] - 2) % 3;
        orientations[moveTable[m][6]] = (orientations[moveTable[m][6]] - 2) % 3;

        if (orientations[moveTable[m][4]] < 0) orientations[moveTable[m][4]] += 3;
        if (orientations[moveTable[m][6]] < 0) orientations[moveTable[m][6]] += 3;
    }
}

bool Cubies::isSolved()
{
    Cubies c;
    for(int i = 0; i < 20; i++)
    {
        if(c.positions[i] != positions[i] || c.orientations[i] != orientations[i])
            return false;
    }
    return true;
}

bool Cubies::operator==(Cubies& cbs)
{
    for (int i = 0; i < 20; i++)
    {
        if (positions[i] != cbs.positions[i] || orientations[i] != cbs.orientations[i])
            return false;
    }
    return true;
}

bool Cubies::HasNext()
{
    return (statesCount > 0);
}

void Cubies::resetNext()
{
    statesCount = 0;
}

/*
shared_ptr<Cubies> Cubies::Copy(shared_ptr<Cubies> b)
{
    shared_ptr<Cubies> _st = shared_ptr<Cubies>(new Cubies());
    for (int i = 0; i < 20; i++)
    {
        _st->positions[i] = b->positions[i];
        _st->orientations[i] = b->orientations[i];
    }
    _st->lastOp = b->lastOp;
    _st->SetPatternTables(b->PtrCornerTable, b->PtrEdge1Table, b->PtrEdge2Table);
    return _st;
}
*/
/*
Cubies* Cubies::Copy(Cubies* b)
{
    Cubies* _st = new Cubies();
    memcpy((void*)_st, (void*)b, sizeof(Cubies));
    /*
    for (int i = 0; i < 20; i++)
    {
        _st->positions[i] = b->positions[i];
        _st->orientations[i] = b->orientations[i];
    }
    _st->lastOp = b->lastOp;
    _st->SetPatternTables(b->PtrCornerTable, b->PtrEdge1Table, b->PtrEdge2Table);
    * /
    return _st;
}
Cubies Cubies::Copy(Cubies b)
{
    Cubies _st ;
    memcpy((void*)&_st, (void*)&b, sizeof(Cubies));
    /*
    for (int i = 0; i < 20; i++)
    {
        _st.positions[i]=  b.positions[i];
        _st.orientations[i] = b.orientations[i];
    }
    _st.lastOp = b.lastOp;
    _st.SetPatternTables(b.PtrCornerTable, b.PtrEdge1Table, b.PtrEdge2Table);
    * /
    return _st;
}
*/

void Cubies::GenerateNextStates(Cubies* NextStates[], short needed)
{
    statesCount = 0; // O(1)
    short tmpHeuristic = this->HeuristicValue; // O(1)
    /* GENERATE CLOCKWISE MOVES */

    //Cubies* _st;
    Cubies* _st; // O(1)

    if (lastOp == -1 || (!AvoidTable[lastOp][0])) // O(1)
    {
        this->clockCycle(moves::U); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 0; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::U) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][1])) // O(1)
    {
        this->clockCycle(moves::F); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 1; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::F) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][2])) // O(1)
    {
        this->clockCycle(moves::D); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 2; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::D) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][3])) // O(1)
    {
        this->clockCycle(moves::B); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 3; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::B) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][4])) // O(1)
    {
        this->clockCycle(moves::R); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 4; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::R) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][5])) // O(1)
    {
        this->clockCycle(moves::L); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 5; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::L) ; // O(1)
    }

    /* GENERATE COUNTERCLOCKWISE MOVES */

    if (lastOp == -1 || (!AvoidTable[lastOp][0 + 6])) // O(1)
    {
        this->counterCycle(moves::U); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 0 + 6; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->clockCycle(moves::U) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][1 + 6])) // O(1)
    {
        this->counterCycle(moves::F); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 1 + 6; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->clockCycle(moves::F) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][2 + 6])) // O(1)
    {
        this->counterCycle(moves::D); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 2 + 6; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->clockCycle(moves::D) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][3 + 6])) // O(1)
    {
        this->counterCycle(moves::B); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 3 + 6; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->clockCycle(moves::B) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][4 + 6])) // O(1)
    {
        this->counterCycle(moves::R); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 4 + 6; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->clockCycle(moves::R) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][5 + 6])) // O(1)
    {
        this->counterCycle(moves::L); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 5 + 6; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->clockCycle(moves::L) ; // O(1)
    }


    /* GENERATE HALF TURNS */

    if (lastOp == -1 || (!AvoidTable[lastOp][0 + 12])) // O(1)
    {
        this->clockCycle(moves::U); // O(1)
        this->clockCycle(moves::U); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 0 + 12; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::U) ; // O(1)
        this->counterCycle(moves::U) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][1 + 12])) // O(1)
    {
        this->clockCycle(moves::F); // O(1)
        this->clockCycle(moves::F); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 1 + 12; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::F) ; // O(1)
        this->counterCycle(moves::F) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][2 + 12])) // O(1)
    {
        this->clockCycle(moves::D); // O(1)
        this->clockCycle(moves::D); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 2 + 12; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::D) ; // O(1)
        this->counterCycle(moves::D) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][3 + 12])) // O(1)
    {
        this->clockCycle(moves::B); // O(1)
        this->clockCycle(moves::B); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 3 + 12; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::B) ; // O(1)
        this->counterCycle(moves::B) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][4 + 12])) // O(1)
    {
        this->clockCycle(moves::R); // O(1)
        this->clockCycle(moves::R); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 4 + 12; // O(1)
        }
        this->HeuristicValue = tmpHeuristic ; // O(1)
        this->counterCycle(moves::R) ; // O(1)
        this->counterCycle(moves::R) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][5 + 12])) // O(1)
    {
        this->clockCycle(moves::L); // O(1)
        this->clockCycle(moves::L); // O(1)
        this->HeuristicValue = -1 ; // O(1)
        if( this->Heuristic() <= needed ) // O(1)
        {
            _st = Copy(this); // O(1)
            NextStates[statesCount++] = _st; // O(1)
            _st->lastOp = 5 + 12; // O(1)
        }
        this->HeuristicValue = tmpHeuristic; // O(1)
        this->counterCycle(moves::L) ; // O(1)
        this->counterCycle(moves::L) ; // O(1)
    }
}

void Cubies::TableGenerateNextStates(Cubies* NextStates[])
{
    statesCount = 0; // O(1)
    short tmpHeuristic = this->HeuristicValue; // O(1)
    /* GENERATE CLOCKWISE MOVES */

    //Cubies* _st;
    Cubies* _st; // O(1)

    if (lastOp == -1 || (!AvoidTable[lastOp][0])) // O(1)
    {
        this->clockCycle(moves::U); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 0; // O(1
        this->counterCycle(moves::U) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][1])) // O(1)
    {
        this->clockCycle(moves::F); // O(1)

        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 1; // O(1)
        this->counterCycle(moves::F) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][2])) // O(1)
    {
        this->clockCycle(moves::D); // O(1)

        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 2; // O(1)
        this->counterCycle(moves::D) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][3])) // O(1)
    {
        this->clockCycle(moves::B); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 3; // O(1)
        this->counterCycle(moves::B) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][4])) // O(1)
    {
        this->clockCycle(moves::R); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 4; // O(1)
        this->counterCycle(moves::R) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][5])) // O(1)
    {
        this->clockCycle(moves::L); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 5; // O(1)
        this->counterCycle(moves::L) ; // O(1)
    }

    /* GENERATE COUNTERCLOCKWISE MOVES */

    if (lastOp == -1 || (!AvoidTable[lastOp][0 + 6])) // O(1)
    {
        this->counterCycle(moves::U); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 0 + 6; // O(1)
        this->clockCycle(moves::U) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][1 + 6])) // O(1)
    {
        this->counterCycle(moves::F); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 1 + 6; // O(1)
        this->clockCycle(moves::F) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][2 + 6])) // O(1)
    {
        this->counterCycle(moves::D); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 2 + 6; // O(1)
        this->clockCycle(moves::D) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][3 + 6])) // O(1)
    {
        this->counterCycle(moves::B); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 3 + 6; // O(1)
        this->clockCycle(moves::B) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][4 + 6])) // O(1)
    {
        this->counterCycle(moves::R); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 4 + 6; // O(1)
        this->clockCycle(moves::R) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][5 + 6])) // O(1)
    {
        this->counterCycle(moves::L); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 5 + 6; // O(1)
        this->clockCycle(moves::L) ; // O(1)
    }


    /* GENERATE HALF TURNS */

    if (lastOp == -1 || (!AvoidTable[lastOp][0 + 12])) // O(1)
    {
        this->clockCycle(moves::U); // O(1)
        this->clockCycle(moves::U); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 0 + 12; // O(1)
        this->counterCycle(moves::U) ; // O(1)
        this->counterCycle(moves::U) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][1 + 12])) // O(1)
    {
        this->clockCycle(moves::F); // O(1)
        this->clockCycle(moves::F); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 1 + 12; // O(1)
        this->counterCycle(moves::F) ; // O(1)
        this->counterCycle(moves::F) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][2 + 12])) // O(1)
    {
        this->clockCycle(moves::D); // O(1)
        this->clockCycle(moves::D); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 2 + 12; // O(1)
        this->counterCycle(moves::D) ; // O(1)
        this->counterCycle(moves::D) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][3 + 12])) // O(1)
    {
        this->clockCycle(moves::B); // O(1)
        this->clockCycle(moves::B); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 3 + 12; // O(1)
        this->counterCycle(moves::B) ; // O(1)
        this->counterCycle(moves::B) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][4 + 12])) // O(1)
    {
        this->clockCycle(moves::R); // O(1)
        this->clockCycle(moves::R); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 4 + 12; // O(1)
        this->counterCycle(moves::R) ; // O(1)
        this->counterCycle(moves::R) ; // O(1)
    }

    if (lastOp == -1 || (!AvoidTable[lastOp][5 + 12])) // O(1)
    {
        this->clockCycle(moves::L); // O(1)
        this->clockCycle(moves::L); // O(1)
        _st = Copy(this); // O(1)
        NextStates[statesCount++] = _st; // O(1)
        _st->lastOp = 5 + 12; // O(1)
        this->counterCycle(moves::L) ; // O(1)
        this->counterCycle(moves::L) ; // O(1)
    }
}

int Cubies::GetCornerHash()
{
    int index;

    int inversions[8] = { 0,0,0,0,0,0,0,0 };
    int positions[8];
    int i, j;

    // Put all the positions into the positions array
    positions[0] = this->positions[12];
    positions[1] = this->positions[13];
    positions[2] = this->positions[14];
    positions[3] = this->positions[15];
    positions[4] = this->positions[16];
    positions[5] = this->positions[17];
    positions[6] = this->positions[18];
    positions[7] = this->positions[19];

    for (i = 0; i < 8; i++)
        for (j = i + 1; j < 8; j++)
            if (positions[i] < positions[j])
                inversions[i]++;

    // the inversion vector is computed. Now compute the permutation index
    // using factoradic numbering
    index  = inversions[0] * 7 * 6 * 5 * 4 * 3 * 2 * 1;
    index += inversions[1] * 6 * 5 * 4 * 3 * 2 * 1;
    index += inversions[2] * 5 * 4 * 3 * 2 * 1;
    index += inversions[3] * 4 * 3 * 2 * 1;
    index += inversions[4] * 3 * 2 * 1;
    index += inversions[5] * 2 * 1;
    index += inversions[6] * 1;
    index += inversions[7]; // * 0!

    index *= 2187; // (3^7)

    // Now compute the orientations and add them into the index
    // treat each rotation value as a digit in base 3
    // skip the last one
    index += orientations[19] * 3 * 3 * 3 * 3 * 3 * 3;
    index += orientations[18] * 3 * 3 * 3 * 3 * 3;
    index += orientations[17] * 3 * 3 * 3 * 3;
    index += orientations[16] * 3 * 3 * 3;
    index += orientations[15] * 3 * 3;
    index += orientations[14] * 3;
    index += orientations[13];

    return index;
}

int Cubies::GetCornerOrientationHash()
{
    int index = 0;

    // Now compute the orientations and add them into the index
    // treat each rotation value as a digit in base 3
    // skip the last one
    index += orientations[19] * 3 * 3 * 3 * 3 * 3 * 3;
    index += orientations[18] * 3 * 3 * 3 * 3 * 3;
    index += orientations[17] * 3 * 3 * 3 * 3;
    index += orientations[16] * 3 * 3 * 3;
    index += orientations[15] * 3 * 3;
    index += orientations[14] * 3;
    index += orientations[13];

    return index;
}

int Cubies::GetEdgesOrientationHash()
{
    int index = 0;
    for (int i = 0; i < 20; i++)
    {
        index *= 10;
        index += orientations[i];
    }
    return index;
}

/*
* This hashes the edge cubies
* 6, 7, 8, 9, 10 and 11
*/
int Cubies::GetEdge1Hash()
{
    Byte positions[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    int index = 0;
    int pos, rot, num, i;

    short cubie;
    short orient;

    /*
    * First split hash space (42577920) into 24 slices:
    * 1774080
    * then into 22 slices:
    * 80640
    * and so forth (each cubie eliminates 2 positions):
    * 4032
    * 224
    * 14
    * 1
    */

    /* first */
    cubie = this->positions[6];
    orient = this->orientations[6];
    num = cubie;	/* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<11; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 12;
    /*
    * pos is range 0-11
    * rot is 0 or 12
    * so pos+rot has range 0-23 as expected
    */
    index += (pos + rot) * 1774080;

    /* second */
    cubie = this->positions[7];
    orient = this->orientations[7];
    num = cubie; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<10; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 11;
    /*
    * pos is range 0-10
    * rot is 0 or 11
    * so pos+rot has range 0-21
    */
    index += (pos + rot) * 80640;

    /* third */
    cubie = this->positions[8];
    orient = this->orientations[8];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<9; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 10;
    /*
    * pos is range 0-9
    * rot is 0 or 10
    * so pos+rot has range 0-19
    */
    index += (pos + rot) * 4032;

    /* fourth */
    cubie = this->positions[9];
    orient = this->orientations[9];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<8; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 9;
    /*
    * pos is range 0-8
    * rot is 0 or 9
    * so pos+rot has range 0-17
    */
    index += (pos + rot) * 224;

    /* fifth */
    cubie = this->positions[10];
    orient = this->orientations[10];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<7; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 8;
    /*
    * pos is range 0-7
    * rot is 0 or 8
    * so pos+rot has range 0-15
    */
    index += (pos + rot) * 14;

    /* sixth */
    cubie = this->positions[11];
    orient = this->orientations[11];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<6; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 7;

    /*
    * pos is range 0-6
    * rot is 0 or 7
    * so pos+rot has range 0-13
    */
    index += (pos + rot);

    return index;
}


/*
* This hashes the edge cubies
* 0, 1, 2, 3, 4 and 5
*/
int Cubies::GetEdge2Hash()
{
    Byte positions[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    int index = 0;
    int pos, rot, num, i;

    short cubie;
    short orient;

    /*
    * First split hash space (42577920) into 24 slices:
    * 1774080
    * then into 22 slices:
    * 80640
    * and so forth (each cubie eliminates 2 positions):
    * 4032
    * 224
    * 14
    * 1
    */

    /* first */
    cubie = this->positions[0];
    orient = this->orientations[0];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<11; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 12;
    /*
    * pos is range 0-11
    * rot is 0 or 12
    * so pos+rot has range 0-23 as expected
    */
    index += (pos + rot) * 1774080;

    /* second */
    cubie = this->positions[1];
    orient = this->orientations[1];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<10; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 11;
    /*
    * pos is range 0-10
    * rot is 0 or 11
    * so pos+rot has range 0-21
    */
    index += (pos + rot) * 80640;

    /* third */
    cubie = this->positions[2];
    orient = this->orientations[2];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<9; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 10;
    /*
    * pos is range 0-9
    * rot is 0 or 10
    * so pos+rot has range 0-19
    */
    index += (pos + rot) * 4032;

    /* fourth */
    cubie = this->positions[3];
    orient = this->orientations[3];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<8; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 9;
    /*
    * pos is range 0-8
    * rot is 0 or 9
    * so pos+rot has range 0-17
    */
    index += (pos + rot) * 224;

    /* fifth */
    cubie = this->positions[4];
    orient = this->orientations[4];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<7; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 8;
    /*
    * pos is range 0-7
    * rot is 0 or 8
    * so pos+rot has range 0-15
    */
    index += (pos + rot) * 14;

    /* sixth */
    cubie = this->positions[5];
    orient = this->orientations[5];
    num = cubie;    /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i = pos; i<6; ++i)
        positions[i] = positions[i + 1];
    rot = orient * 7;
    /*
    * pos is range 0-6
    * rot is 0 or 7
    * so pos+rot has range 0-13
    */
    index += (pos + rot);

    return index;
}


Byte Cubies::Heuristic()
{
    if (HeuristicValue != -1) return HeuristicValue;

    int hashCorner = GetCornerHash();
    int Edge1Hash = GetEdge1Hash();
    int Edge2Hash = GetEdge2Hash();

    Byte cornerH; //= CornerTableLookup(hashCorner);
    if (hashCorner & 1) cornerH = PtrCornerTable[(hashCorner - 1) >> 1] >> 4;
    else cornerH = PtrCornerTable[hashCorner / 2] & 15;

    Byte Edge1H; //= Edge1TableLookup(Edge1Hash);
    if (Edge1Hash & 1) Edge1H = PtrEdge1Table[(Edge1Hash - 1) >> 1] >> 4;
    else Edge1H = PtrEdge1Table[Edge1Hash / 2] & 15;


    Byte Edge2H; //= Edge2TableLookup(Edge2Hash);
    if (Edge2Hash & 1) Edge2H = PtrEdge2Table[(Edge2Hash - 1) >> 1] >> 4;
    else Edge2H = PtrEdge2Table[Edge2Hash / 2] & 15;

    HeuristicValue = cornerH;
    if (Edge1H > HeuristicValue) HeuristicValue = Edge1H;
    if (Edge2H > HeuristicValue) HeuristicValue = Edge2H;

    return HeuristicValue;
}


Byte Cubies::CornerTableLookup(int hash)
{
    Byte res;
    if (hash & 1) res = PtrCornerTable[(hash - 1) >> 1] >> 4;
    else res = PtrCornerTable[hash / 2] & 15;
    return res;
}

Byte Cubies::Edge1TableLookup(int hash)
{
    Byte res;
    if (hash & 1) res = PtrEdge1Table[(hash - 1) >> 1] >> 4;
    else res = PtrEdge1Table[hash / 2] & 15;
    return res;
}

Byte Cubies::Edge2TableLookup(int hash)
{
    Byte res;
    if (hash & 1) res = PtrEdge2Table[(hash - 1) >> 1] >> 4;
    else res = PtrEdge2Table[hash / 2] & 15;
    return res;
}

#include "stdafx.h"
#include "Cubies.h"

Cubies::Cubies()
{
	//assume is solved
	CurrentStage = 0;
	for(short i = 0; i < 20; i++)
	{
		positions[i] = i;
		orientations[i] = 0;
	}
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



void Cubies::clockCycle(moves m)
{

	// edges

	short tmpPos = positions[moveGen.moveTable[m][3]];
	short tmpOr = orientations[moveGen.moveTable[m][3]];

	for(int i = 3; i > 0; i-- )
	{
		positions[moveGen.moveTable[m][i]] = positions[moveGen.moveTable[m][i - 1]];
		orientations[moveGen.moveTable[m][i]] = orientations[moveGen.moveTable[m][i - 1]]; 
	}
	positions[moveGen.moveTable[m][0]] = tmpPos;
	orientations[moveGen.moveTable[m][0]] = tmpOr;

	// F, B affects edges orientation (if 0 (correct) -> 1 (incorrect))

	if(m < 2)
		for(int i = 0; i < 4; i++)
		{
			orientations[moveGen.moveTable[m][i]] = (orientations[moveGen.moveTable[m][i]] == 1)? 0 : 1;
		}

	// corners

	tmpPos = positions[moveGen.moveTable[m][7]];
	tmpOr = orientations[moveGen.moveTable[m][7]];

	for(int i = 7; i > 4; i-- )
	{
		positions[moveGen.moveTable[m][i]] = positions[moveGen.moveTable[m][i - 1]];
		orientations[moveGen.moveTable[m][i]] = orientations[moveGen.moveTable[m][i - 1]]; 
	}
	positions[moveGen.moveTable[m][4]] = tmpPos;
	orientations[moveGen.moveTable[m][4]] = tmpOr;


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

		orientations[moveGen.moveTable[m][5]] = (orientations[moveGen.moveTable[m][5]] + 2) % 3;
		orientations[moveGen.moveTable[m][7]] = (orientations[moveGen.moveTable[m][7]] + 2) % 3;

		// cw

		orientations[moveGen.moveTable[m][4]] = (orientations[moveGen.moveTable[m][4]] + 1) % 3;
		orientations[moveGen.moveTable[m][6]] = (orientations[moveGen.moveTable[m][6]] + 1) % 3;

	}


}


void Cubies::counterCycle(moves m)
{

	// edges

	short tmpPos = positions[moveGen.moveTable[m][0]];
	short tmpOr = orientations[moveGen.moveTable[m][0]];

	for(int i = 0; i < 3; i++ )
	{
		positions[moveGen.moveTable[m][i]] = positions[moveGen.moveTable[m][i + 1]];
		orientations[moveGen.moveTable[m][i]] = orientations[moveGen.moveTable[m][i + 1]]; 
	}
	positions[moveGen.moveTable[m][3]] = tmpPos;
	orientations[moveGen.moveTable[m][3]] = tmpOr;

	if(m < 2)
		for(int i = 0; i < 4; i++)
		{
			orientations[moveGen.moveTable[m][i]] = (orientations[moveGen.moveTable[m][i]] == 1)? 0 : 1;
		}
	

	// corners

	tmpPos = positions[moveGen.moveTable[m][4]];
	tmpOr = orientations[moveGen.moveTable[m][4]];

	for(int i = 4; i < 7; i++ )
	{
		positions[moveGen.moveTable[m][i]] = positions[moveGen.moveTable[m][i + 1]];
		orientations[moveGen.moveTable[m][i]] = orientations[moveGen.moveTable[m][i + 1]]; 
	}
	positions[moveGen.moveTable[m][7]] = tmpPos;
	orientations[moveGen.moveTable[m][7]] = tmpOr;

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

		orientations[moveGen.moveTable[m][5]] = (orientations[moveGen.moveTable[m][5]] + 1) % 3;
		orientations[moveGen.moveTable[m][7]] = (orientations[moveGen.moveTable[m][7]] + 1) % 3;

		// ccw

		orientations[moveGen.moveTable[m][4]] = (orientations[moveGen.moveTable[m][4]] + 2) % 3;
		orientations[moveGen.moveTable[m][6]] = (orientations[moveGen.moveTable[m][6]] + 2) % 3;

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

short Cubies::getPosition(int idx)
{
	return positions[idx];
}

short Cubies::getOrientation(int idx)
{
	return orientations[idx];
}

void Cubies::setPosition(int idx, short val)
{
	positions[idx] = val;
}

void Cubies::setOrientation(int idx, short val)
{
	orientations[idx] = val;
}

int Cubies::Heuristic()
{
	return 0;
}

bool Cubies::operator==(Cubies& cbs)
{
	for (int i = 0; i < 20; i++)
	{
		if (positions[i] != cbs.getPosition(i) || orientations[i] != cbs.getOrientation(i))
			return false;
	}
	return true;
}

bool Cubies::HasNext()
{
	return (NextStates.size() > 0);
}

void Cubies::resetNext()
{
	NextStates.clear();
	GenerateNextStates();
}

shared_ptr<Cubies> Cubies::Copy(shared_ptr<Cubies> b)
{
	shared_ptr<Cubies> _st = shared_ptr<Cubies>(new Cubies());
	//_st.State = b.State;
	for (int i = 0; i < 20; i++)
	{
		_st->setPosition(i, b->getPosition(i));
		_st->setOrientation(i, b->getOrientation(i));
	}
	_st->CurrentStage = b->CurrentStage;
	_st->moveGen = Move();
	return _st;
}

shared_ptr<Cubies> Cubies::Copy(Cubies* b)
{
	shared_ptr<Cubies> _st = shared_ptr<Cubies>(new Cubies());
	//_st.State = b.State;
	for (int i = 0; i < 20; i++)
	{
		_st->setPosition(i, b->getPosition(i));
		_st->setOrientation(i, b->getOrientation(i));
	}
	_st->CurrentStage = b->CurrentStage;
	_st->moveGen = Move();
	return _st;
}

shared_ptr<Cubies> Cubies::Copy(Cubies& b)
{
	shared_ptr<Cubies> _st = shared_ptr<Cubies>(new Cubies());
	//_st.State = b.State;
	for (int i = 0; i < 20; i++)
	{
		_st->setPosition(i, b.getPosition(i));
		_st->setOrientation(i, b.getOrientation(i));
	}
	_st->CurrentStage = b.CurrentStage;
	_st->moveGen = Move();
	return _st;
}

void Cubies::GenerateNextStates()
{
	for (int i = 0; i < moveGen.stages[CurrentStage].size(); i++)
	{
		shared_ptr<Cubies> _st = shared_ptr<Cubies>(new Cubies());;
		_st = Copy(this);

		if (moveGen.stages[CurrentStage][i] == 'F')
			_st->clockCycle(moves::F);
		if (moveGen.stages[CurrentStage][i] == 'U')
			_st->clockCycle(moves::U);
		if (moveGen.stages[CurrentStage][i] == 'D')
			_st->clockCycle(moves::D);
		if (moveGen.stages[CurrentStage][i] == 'L')
			_st->clockCycle(moves::L);
		if (moveGen.stages[CurrentStage][i] == 'R')
			_st->clockCycle(moves::R);
		if (moveGen.stages[CurrentStage][i] == 'B')
			_st->clockCycle(moves::B);

		NextStates.push_back(_st);
	}
}

shared_ptr<Cubies> Cubies::NextState()
{
	shared_ptr<Cubies> _st;
	_st = NextStates[0];
	NextStates.erase(NextStates.begin());

	return _st;
}

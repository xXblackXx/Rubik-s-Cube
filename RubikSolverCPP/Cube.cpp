#include "stdafx.h"
#include "Cube.h"

// the sting is (0-9) up front down back right left

Cube::Cube(string s)
{
	strToCube(s);	
}

void Cube::strToCube(string s)
{
	int i = 0;
	center[0] = s[i+4];
	up = Face(s.substr(i, 9));
	up.setSide(FaceName::up);
	i += 9;
	center[1] = s[i+4];
	front = Face(s.substr(i, 9));
	front.setSide(FaceName::front);
	i += 9;
	center[2] = s[i+4];
	down = Face(s.substr(i, 9));
	down.setSide(FaceName::down);
	i += 9;
	center[3] = s[i+4];
	back = Face(s.substr(i, 9));
	back.setSide(FaceName::back);
	i += 9;
	center[4] = s[i+4];
	right = Face(s.substr(i, 9));
	right.setSide(FaceName::right);
	i += 9;
	center[5] = s[i+4];
	left = Face(s.substr(i, 9));
	left.setSide(FaceName::left);
	i += 9;
}

char Cube::colorToFaceLetter(char c)
{
	switch(c)
    {
        	case center[1]:
        		return 'F';
        	case center[5]:
        		return 'L';
	        case center[4]:
	        	return 'R';
	        case center[0]:
	        	return 'U';
	        case center[2]: 
	        	return 'D';
	        case center[3]: 
	        	return 'B';
    }
}*
bool Cube::operator== (const Cube& c)
{
	if(up != c.up)
		return false;
	if(front != c.front)
		return false;
	if(down != c.down)
		return false;
	if(back != c.back)
		return false;
	if(right != c.right)
		return false;
	if(left != c.left)
		return false;
	return true;
}

bool Cube::operator!=(const Cube& c)
{
	return !(*this == c);
}

void Cube::toSolved()
{
	strToCube(solvedCubeInit(center[6]));
}

bool Cube::isSolved()
{
	Cube c(solvedCubeInit(center[6]));
	return (*this == c);
}

		
Face Cube::getFace(FaceName f)
{
	switch(f)
	{
		case FaceName::up:
			return up;
		case FaceName::down:
			return down;
		case FaceName::left:
			return left;
		case FaceName::right:
			return right;
		case FaceName::front:
			return front;
		case FaceName::back:
			return back;
	}
}

FaceName Cube::getActualFace(FaceName f)
{
	return getFace(f).getSide();
}

void Cube::setSide(FaceName f, FaceName x)
{
	switch(f)
	{
		case FaceName::up:
			up.setSide(x);
			break;
		case FaceName::down:
			down.setSide(x);
			break;
		case FaceName::left:
			left.setSide(x);
			break;
		case FaceName::right:
			right.setSide(x);
			break;
		case FaceName::front:
			front.setSide(x);
			break;
		case FaceName::back:
			back.setSide(x);
			break;
	}
}



void Cube::setFace(Face f, FaceName x)
{
	switch (x)
	{
	case FaceName::up:
		up.setUsingFace(f);
		break;
	case FaceName::down:
		down.setUsingFace(f);
		break;
	case FaceName::left:
		left.setUsingFace(f);
		break;
	case FaceName::right:
		right.setUsingFace(f);
		break;
	case FaceName::front:
		front.setUsingFace(f);
		break;
	case FaceName::back:
		back.setUsingFace(f);
		break;
	}
}

// using colorToFaceLetter (utility.h) to get cubies names 

string Cube::fromColorToEdge(Color c1, Color c2)
{
	string tmp = "";
	tmp += colorToFaceLetter(c1);
	tmp += colorToFaceLetter(c2);
	return tmp;
}
		

string Cube::fromColorToCorner(Color c1, Color c2, Color c3)
{
	string tmp = "";
	tmp += colorToFaceLetter(c1);
	tmp += colorToFaceLetter(c2);
	tmp += colorToFaceLetter(c3);
	return tmp;
}

Cubie Cube::toCubie(string s)
{
	if(s.size() == 2)
		return fromStringToEdge(s);
	else
		return fromStringToCorner(s);
}


short Cube::getOrientation(string s)
{
	short orientation = 0;
	for(short i = 0; i < 6; i++)
	{
		if(contains(s, order[i]))
		{
			for(short j = 0; j < s.size(); j++)
			{
				if(s[j] == order[i])
					return j;
			}
		}
	}
}


Cubies Cube::toCubiesFromSides()
{
	Cubies cube;
	string cubies[20];

	// edges
		//FU 
	cubies[0] = fromColorToEdge(front[1], up[7]);
	//FD
	cubies[1] = fromColorToEdge(front[7], down[1]);
	//FR
	cubies[2] = fromColorToEdge(front[5], right[3]);
	//FL
	cubies[3] = fromColorToEdge(front[3], left[5]);
	//UR
	cubies[4] = fromColorToEdge(up[5], right[1]);
	//UL
	cubies[5] = fromColorToEdge(up[3], left[1]);
	//UB
	cubies[6] = fromColorToEdge(up[1], back[1]);
	//DR
	cubies[7] = fromColorToEdge(down[5], right[7]);
	//DL
	cubies[8] = fromColorToEdge(down[3], left[7]);
	//DB
	cubies[9] = fromColorToEdge(down[7], back[7]);
	//BR
	cubies[10] = fromColorToEdge(back[3], right[5]);
	//BL
	cubies[11] = fromColorToEdge(back[5], left[3]);

	// corners
		//FUR
	cubies[12] = fromColorToCorner(front[2], up[8], right[0]);
	//FUL
	cubies[13] = fromColorToCorner(front[0], up[6], left[2]);
	//FDR
	cubies[14] = fromColorToCorner(front[8], down[2], right[6]);
	//FDL
	cubies[15] = fromColorToCorner(front[6], down[0], left[8]);
	//UBR
	cubies[16] = fromColorToCorner(up[2], back[0], right[2]);
	//UBl
	cubies[17] = fromColorToCorner(up[0], back[2], left[0]);
	//DBR
	cubies[18] = fromColorToCorner(down[8], back[6], right[8]);
	//DBL
	cubies[19] = fromColorToCorner(down[6], back[8], left[6]);


	// get positions


	for (int i = 0; i < 20; i++)
	{
		cube.setPosition(i, short(toCubie(cubies[i])));
		cube.setOrientation(i, getOrientation(cubies[i]));
	}

	return cube;
}

Cubies Cube::CubeFromMovesList(string* moveList, int len)
{
	Cube c = Cube(solvedCube);
	Cubies cbs = c.toCubiesFromSides();

	for (int i = 0; i < len; i++)
	{
		if (moveList[i] == "U")
		{
			cbs.clockCycle(moves::U);
		}
		else if (moveList[i] == "D")
		{
			cbs.clockCycle(moves::D);
		}
		else if (moveList[i] == "L")
		{
			cbs.clockCycle(moves::L);
		}
		else if (moveList[i] == "R")
		{
			cbs.clockCycle(moves::R);
		}
		else if (moveList[i] == "F")
		{
			cbs.clockCycle(moves::F);
		}
		else if (moveList[i] == "B")
		{
			cbs.clockCycle(moves::B);
		}
		else if (moveList[i] == "U'")
		{
			cbs.counterCycle(moves::U);
		}
		else if (moveList[i] == "D'")
		{
			cbs.counterCycle(moves::D);
		}
		else if (moveList[i] == "L'")
		{
			cbs.counterCycle(moves::L);
		}
		else if (moveList[i] == "R'")
		{
			cbs.counterCycle(moves::R);
		}
		else if (moveList[i] == "F'")
		{
			cbs.counterCycle(moves::F);
		}
		else if (moveList[i] == "B'")
		{
			cbs.counterCycle(moves::B);
		}
		else if (moveList[i] == "U2")
		{
			cbs.clockCycle(moves::U);
			cbs.clockCycle(moves::U);
		}
		else if (moveList[i] == "D2")
		{
			cbs.clockCycle(moves::D);
			cbs.clockCycle(moves::D);
		}
		else if (moveList[i] == "L2")
		{
			cbs.clockCycle(moves::L);
			cbs.clockCycle(moves::L);
		}
		else if (moveList[i] == "R2")
		{
			cbs.clockCycle(moves::R);
			cbs.clockCycle(moves::R);
		}
		else if (moveList[i] == "F2")
		{
			cbs.clockCycle(moves::F);
			cbs.clockCycle(moves::F);
		}
		else if (moveList[i] == "B2")
		{
			cbs.clockCycle(moves::B);
			cbs.clockCycle(moves::B);
		}
	}
	return cbs;
}
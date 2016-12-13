#include "cube.h"

// the sting is (0-9) up front down back right left

Cube::Cube(string s)
{
	strToCube(s);
}

Cube::~Cube() {}

void Cube::strToCube(string s)
{
	int i = 0;
	center[0] = s[i + 4];
	up = Face(s.substr(i, 9));
	up.setSide(FaceName::up);
	i += 9;
	center[1] = s[i + 4];
	front = Face(s.substr(i, 9));
	front.setSide(FaceName::front);
	i += 9;
	center[2] = s[i + 4];
	down = Face(s.substr(i, 9));
	down.setSide(FaceName::down);
	i += 9;
	center[3] = s[i + 4];
	back = Face(s.substr(i, 9));
	back.setSide(FaceName::back);
	i += 9;
	center[4] = s[i + 4];
	right = Face(s.substr(i, 9));
	right.setSide(FaceName::right);
	i += 9;
	center[5] = s[i + 4];
	left = Face(s.substr(i, 9));
	left.setSide(FaceName::left);
	i += 9;
}
/*
string Cube::fromCubiesToCubeString(Cubies& cubie)
{
    string cube = cubie.toStringOfFaces();
    string tmp = "";
    cube = faceLetterToColor(cube);

    // put colors in the correct face
    // up front down back right left

    // up
           tmp += cube[39] + cube[12] + cube[36] + cube[10]
                + center[0]
                + cube[8] + cube[28] + cube[1] + cube[5];

    // front
           tmp += cube[27] + cube[0] + cube[24] + cube[6]
                + center[1]
                + cube[4] + cube[33] + cube[2] + cube[30];

    // down
           tmp += cube[34] + cube[3] + cube[31] + cube[16]
                + center[2]
                + cube[14] + cube[45] + cube[18] + cube[42];

    // back
           tmp += cube[37] + cube[13] + cube[40] + cube[20]
                + center[3]
                + cube[22] + cube[43] + cube[19] + cube[46];

    // right
           tmp += cube[26] + cube[9] + cube[38] + cube[5]
                + center[4]
                + cube[21] + cube[32] + cube[15] + cube[44];

    // left
           tmp += cube[41] + cube[11] + cube[29] + cube[23]
                + center[5]
                + cube[7] + cube[47] + cube[17] + cube[35];
   return tmp;
}*/

char Cube::colorToFaceLetter(Color c)
{
	Color temp[6];
	for (int i = 0; i < 6; i++)
	{
		if (center[i] == 'y') temp[i] = Color::yellow;
		if (center[i] == 'b') temp[i] = Color::blue;
		if (center[i] == 'g') temp[i] = Color::green;
		if (center[i] == 'o') temp[i] = Color::orange;
		if (center[i] == 'r') temp[i] = Color::red;
		if (center[i] == 'w') temp[i] = Color::white;
	}
	if (temp[1] == c) return 'F';
	else if (temp[5] == c) return 'L';
	else if (temp[4] == c) return 'R';
	else if (temp[0] == c) return 'U';
	else if (temp[2] == c) return 'D';
	else if (temp[3] == c) return 'B';
}

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
	strToCube(solvedCube);
}

bool Cube::isSolved()
{
	Cube c(solvedCube);
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
	for(int i = 0; i < 6; i++)
	{
		if(contains(s, order[i]))
		{
			for(int j = 0; j < s.size(); j++)
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
    // for orientation purpose string generated as UF
    cubies[0] = fromColorToEdge(up[7] ,front[1]);
    //FD
    // for orientation purpose string generated as DF
    cubies[1] = fromColorToEdge(down[1], front[7]);
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
    // for orientation purpose string generated as UFR
    cubies[12] = fromColorToCorner(up[8], front[2], right[0]);
    //FUL
    // for orientation purpose string generated as ULF
    cubies[13] = fromColorToCorner(up[6], left[2], front[0]);
    //FDR
    // for orientation purpose string generated as DRF
    cubies[14] = fromColorToCorner(down[2], right[6], front[8]);
    //FDL
    // for orientation purpose string generated as DFL
    cubies[15] = fromColorToCorner(down[0], front[6], left[8]);
    //UBR
    // for orientation purpose string generated as URB
    cubies[16] = fromColorToCorner(up[2], right[2], back[0]);
    //UBl
    cubies[17] = fromColorToCorner(up[0], back[2], left[0]);
    //DBR
    cubies[18] = fromColorToCorner(down[8], back[6], right[8]);
    //DBL
    // for orientation purpose string generated as DLB
    cubies[19] = fromColorToCorner(down[6], left[6], back[8]);

	// get positions

	for (int i = 0; i < 20; i++)
	{
		cube.positions[i] = short(toCubie(cubies[i]));
		cube.orientations[i] = getOrientation(cubies[i]);
	}

	return cube;
}


Cubies Cube::CubeFromMovesList(string* moveList, int len)
{
	Cube c = Cube(solvedCube);
	Cubies cbs = c.toCubiesFromSides();

    //for (int i = 0;i < 20; i++) cout << cbs.positions[i] << " ";
        //cout <<endl;
        //for (int i = 0;i < 20; i++) cout << cbs.orientations[i] << " ";
        //cout <<endl;

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
		//for (int i = 0;i < 20; i++) cout << cbs.positions[i] << " ";
        //cout <<endl;
        //for (int i = 0;i < 20; i++) cout << cbs.orientations[i] << " ";
        //cout <<endl;
	}
	return cbs;
}

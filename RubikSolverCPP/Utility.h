#pragma once
#include "stdafx.h"
#include <iostream>

using namespace std;




enum Color
{
	white = 0, // up face
	yellow, // down
	blue, // left
	green, // right
	red,	// front
	orange, // back
};

enum FaceName
{
	up = 0,
	down,
	left,
	right,
	front, 
	back
};


inline Color charToCol(char s)
{
	switch(s)
	{
		case 'w':
			return Color::white;
		case 'y':
			return Color::yellow;
		case 'b':
			return Color::blue;
		case 'g':
			return Color::green;
		case 'r':
			return Color::red;
		case 'o':
			return Color::orange;
	}
}

enum Cubie  // edge ->  2 faces of the cubie appear , corner -> 3 faces
{
	// 12 edge 

		FU = 0,
		FD, // 1
		FR, // 2
		FL, // 3
		UR, // 4
		UL, // 5
		UB, // 6
		DR, // 7
		DL, // 8
		DB, // 9
		BR, // 10
		BL, // 11

	// 8 corners

		FUR, // 12
		FUL, // 13
		FDR, // 14
		FDL, // 15
		UBR, // 16
		UBl, // 17
		DBR, // 18
		DBL  // 19

	// total 20
};

// notice in the above enum i used an order F -> U -> D -> B -> R -> L
// the order of R and L doesn't matter, they can't occur together 
// this order will be used in orientation


inline Cubie fromStringToEdge(string s)
{
	if((s[0] == 'F' && s[1] == 'U') ^ (s[0] == 'U' && s[1] == 'F'))
		return Cubie::FU;
	else if((s[0] == 'F' && s[1] == 'D') ^ (s[0] == 'D' && s[1] == 'F'))
		return Cubie::FD;
	else if((s[0] == 'F' && s[1] == 'R') ^ (s[0] == 'R' && s[1] == 'F'))
		return Cubie::FR;
	else if((s[0] == 'F' && s[1] == 'L') ^ (s[0] == 'L' && s[1] == 'F'))
		return Cubie::FL;
	else if((s[0] == 'R' && s[1] == 'U') ^ (s[0] == 'U' && s[1] == 'R'))
		return Cubie::UR;
	else if((s[0] == 'L' && s[1] == 'U') ^ (s[0] == 'U' && s[1] == 'L'))
		return Cubie::UL;
	else if((s[0] == 'B' && s[1] == 'U') ^ (s[0] == 'U' && s[1] == 'B'))
		return Cubie::UB;
	else if((s[0] == 'D' && s[1] == 'R') ^ (s[0] == 'R' && s[1] == 'D'))
		return Cubie::DR;
	else if((s[0] == 'D' && s[1] == 'L') ^ (s[0] == 'L' && s[1] == 'D'))
		return Cubie::DL;
	else if((s[0] == 'D' && s[1] == 'B') ^ (s[0] == 'B' && s[1] == 'D'))
		return Cubie::DB;
	else if((s[0] == 'B' && s[1] == 'R') ^ (s[0] == 'R' && s[1] == 'B'))
		return Cubie::BR;
	else if((s[0] == 'B' && s[1] == 'L') ^ (s[0] == 'L' && s[1] == 'B'))
		return Cubie::BL;
	return Cubie::FU;
}

// this's only for corners
inline bool contains(string s, char c)
{
	for(int i = 0; i < s.size(); i++)
		if(s[i] == c)
			return true;
	return false;
}

inline Cubie fromStringToCorner(string s)
{
	if(contains(s,'F'))
	{
		if(contains(s, 'U'))
		{
			if(contains(s, 'R'))
				return Cubie::FUR;
			else //if (contains(s, 'L'))
				return Cubie::FUL;
		}
		else // if (contains(s, 'D')) 
		{
			if(contains(s, 'R'))
				return Cubie::FDR;
			else // if(contains(s,'L'))
				return Cubie::FDL;
		}
	}
	else if(contains(s, 'U'))
	{
		// should contain B

		if(contains(s, 'R'))
			return Cubie::UBR;
		else // if(contains(s, 'L'))
			return Cubie::UBl;
	}
	else if(contains(s, 'D'))
	{
		// should contain B

		if(contains(s, 'R'))
			return Cubie::DBR;
		else // if(contains(s, 'L'))
			return Cubie::DBL;
	}
	else
		return Cubie::FUL;
}

enum moves
{
	F = 0,
	B,
	R,
	L,
	U,
	D
};

inline char fromMovesToChar(moves m)
{
	switch(m)
	{
			case F:
        		return 'F';
        	case B:
        		return 'B';
	        case R:
	        	return 'R';
	        case L:
	        	return 'L';
	        case U: 
	        	return 'U';
	        case D: 
	        	return 'D';

	}
}

inline FaceName colorToLocation(Color c)
 {
    switch(c)
    {
        	case white:
        		return FaceName::front;
        	case orange:
        		return FaceName::left;
	        case red:
	        	return FaceName::right;
	        case yellow:
	        	return FaceName::up;
	        case green: 
	        	return FaceName::down;
	        case blue: 
	        	return FaceName::back;
    }
  }

// ex for following function : white cubletes should be on front face in solved cube 

/*inline char colorToFaceLetter(char c)
{
	switch(c)
    {
        	case 'w':
        		return 'F';
        	case 'o':
        		return 'L';
	        case 'r':
	        	return 'R';
	        case 'y':
	        	return 'U';
	        case 'g': 
	        	return 'D';
	        case 'b': 
	        	return 'B';
    }
}*/

inline char colorToFaceLetter(Color c)
{
	switch(c)
    {
        	case white:
        		return 'F';
        	case orange:
        		return 'L';
	        case red:
	        	return 'R';
	        case yellow:
	        	return 'U';
	        case green: 
	        	return 'D';
	        case blue: 
	        	return 'B';
    }
}

 // the sting is (0-9) up front down back right left

const string solvedCube = "yyyyyyyyywwwwwwwwwgggggggggbbbbbbbbbrrrrrrrrrooooooooo";

// moves to the whole cube not cubies

// enum Actions
// {
// 	flip = 0,
	
// }


// pass a string of 6 letters of the colors to define "up front down back right left"

inline string solvedCubeInit(char s[6])
{
	string solutioin = "";

	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 9 ; j++)
			solutioin += s[i];
	}
	return solutioin;
}

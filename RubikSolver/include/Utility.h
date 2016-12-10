#pragma once

#ifndef UTILITY_H
#define UTILITY_H

#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>

typedef unsigned char Byte;

#define CORNERTABLESIZE 44089920
#define EDGETABLESIZE 21288960


using namespace std;

// http://www.jaapsch.net/puzzles/thistle.htm
// allowes moves for stages
// if a move is not present in the stage it should be 2*move
//string stages[4] =
//{
//	"FBUDRL",
//	"UDRL",
//	"UD",
//	""
//};

enum moves
{
	F = 0,
	B,
	R,
	L,
	U,
	D
};

static bool AvoidTable[18][18] =
{
	/*	 HALF		   CCW		    CW
		UFDBRL		  UFDBRL	   UFDBRL
	|17<---------------------------------->0|*/
	{ 1,0,0,0,0,0, 1,0,0,0,0,0, 1,0,0,0,0,0 }, //U
	{ 0,1,0,0,0,0, 0,1,0,0,0,0, 0,1,0,0,0,0 }, //F
	{ 1,0,1,0,0,0, 1,0,1,0,0,0, 1,0,1,0,0,0 }, //D
	{ 0,1,0,1,0,0, 0,1,0,1,0,0, 0,1,0,1,0,0 }, //B    //CW
	{ 0,0,0,0,1,1, 0,0,0,0,1,1, 0,0,0,0,1,1 }, //R
	{ 0,0,0,0,0,1, 0,0,0,0,0,1, 0,0,0,0,0,1 }, //L

	{ 1,0,0,0,0,0, 1,0,0,0,0,0, 1,0,0,0,0,0 }, //U
	{ 0,1,0,0,0,0, 0,1,0,0,0,0, 0,1,0,0,0,0 }, //F
	{ 1,0,1,0,0,0, 1,0,1,0,0,0, 1,0,1,0,0,0 }, //D
	{ 0,1,0,1,0,0, 0,1,0,1,0,0, 0,1,0,1,0,0 }, //B    //CCW
	{ 0,0,0,0,1,1, 0,0,0,0,1,1, 0,0,0,0,1,1 }, //R
	{ 0,0,0,0,0,1, 0,0,0,0,0,1, 0,0,0,0,0,1 }, //L

	{ 1,0,0,0,0,0, 1,0,0,0,0,0, 1,0,0,0,0,0 }, //U
	{ 0,1,0,0,0,0, 0,1,0,0,0,0, 0,1,0,0,0,0 }, //F
	{ 1,0,1,0,0,0, 1,0,1,0,0,0, 1,0,1,0,0,0 }, //D
	{ 0,1,0,1,0,0, 0,1,0,1,0,0, 0,1,0,1,0,0 }, //B    //HALF-TURNS
	{ 0,0,0,0,1,1, 0,0,0,0,1,1, 0,0,0,0,1,1 }, //R
	{ 0,0,0,0,0,1, 0,0,0,0,0,1, 0,0,0,0,0,1 }, //L
};

static short moveTable[6][8] =
{
	{/*edge*/   0,2,1,3, /*corner*/  13, 12, 14, 15 },  // move F

														// index of moveTable in face
														//	 ___ ___ ___
														//	|4	|0 	|5	|
														//	|___|___|___|
														//	|3	|   |1	|
														//	|___|___|___|
														//	|7	|2	| 6	|
														//	|___|___|___|

	{ 6, 11, 9, 10, 16, 17, 19, 18 }, // B
	{ 4, 10, 7, 2, 12, 16, 18, 14 },  // R
	{ 5, 3, 8, 11, 17, 13, 15, 19 }, // L
	{ 0, 5, 6, 4, 12, 13, 17, 16 }, // U
	{ 1, 7, 9, 8, 15, 14, 18, 19 } // D
};


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
/*
inline char colorToFaceLetter(char c)
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
}
*/

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

inline string solvedCubeInit(string s)
{
	string solutioin = "";

	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 9 ; j++)
			solutioin += s[i];
	}
	return solutioin;
}

inline void InitPatternTables(Byte* CORNER_TABLE, Byte* EDGE1_TABLE, Byte* EDGE2_TABLE)
{
	memset(CORNER_TABLE, 255, CORNERTABLESIZE);
	memset(EDGE1_TABLE, 255, EDGETABLESIZE);
	memset(EDGE2_TABLE, 255, EDGETABLESIZE);
}

inline bool LoadPatternTables(Byte* CORNER_TABLE, Byte* EDGE1_TABLE, Byte* EDGE2_TABLE)
{
	FILE *input;
	bool exist = true;
	if (input = fopen("table_corner.tab", "r"))
		fclose(input);
	else exist = false;

	if (input = fopen("table_edge1.tab", "r"))
		fclose(input);
	else exist = false;

	if (input = fopen("table_edge2.tab", "r"))
		fclose(input);
	else exist = false;

	if (!exist)
	{
		//TODO: GENERATE TABLES
	}
	else
	{
		if (input = fopen("table_corner.tab", "rb"))
		{
			int read = fread(CORNER_TABLE, 1, CORNERTABLESIZE, input);
			cout << "Corner Table Size: " << read << endl;
			exist = true;
			fclose(input);
		}

		if (input = fopen("table_edge1.tab", "rb"))
		{
			int read = fread(EDGE1_TABLE, 1, EDGETABLESIZE, input);
			cout << "Edge 1 Table Size: " << read << endl;
			exist = true;
			fclose(input);
		}

		if (input = fopen("table_edge2.tab", "rb"))
		{
			int read = fread(EDGE2_TABLE, 1, EDGETABLESIZE, input);
			cout << "Edge 2 Table Size: " << read << endl;
			exist = true;
			fclose(input);
		}
	}
	return exist;
}

#endif // UTILITY_H

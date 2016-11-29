#pragma once 
#include <iostream>
#include "utility.h"

using namespace std;

class Move
{
	public:
		// http://www.jaapsch.net/puzzles/thistle.htm
		// allowes moves for stages 
		// if a move is not present in the stage it should be 2*move
		string stages[4] = 
		{
			"FBUDRL",
			"UDRL",
			"UD",
			""
		};
		short moveTable[6][8] =
		{
			{/*edge*/   0,2,1,3, /*corner*/  13, 12, 14, 15},  // move F

			// index of moveTable in face
			//	 ___ ___ ___
			//	|4	|0 	|5	|
			//	|___|___|___|
			//	|3	|   |1	|
			//	|___|___|___|
			//	|7	|2	| 6	|
			//	|___|___|___|

			{6, 11, 9, 10, 16, 17, 19, 18}, // B
			{4, 10, 7, 2, 12, 16, 18, 14},  // R
			{5, 3, 8, 11, 17, 13, 15, 19}, // L
			{0, 5, 6, 4, 12, 13, 17, 16}, // U
			{1, 7, 9, 8, 15, 14, 18, 19} // D
		};
			
		moves m;

		moves opposite(moves m); // U is the opposite of D (0, 2, 4) (1, 3, 5)
};
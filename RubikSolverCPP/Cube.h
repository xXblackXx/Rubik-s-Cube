#pragma once
#include "stdafx.h"
#include <iostream>
#include <string>
#include "face.h"
#include "utility.h"
#include "Cubies.h"

using namespace std;

// for reading purposes only 
// class cubies will be used for solving
// we can use opencv to read a cube
// then give string of moves as answer

class Cube
{
	private:
		Face up, down, front, back, right, left;
		string order = "FUDBRL";
		char center[6];

	public:
		
		Cube(string s);
		void strToCube(string s);

		char colorToFaceLetter(char c);

		bool operator == (const Cube& c);
		bool operator != (const Cube& c);

		void toSolved();
		bool isSolved();

		Face getFace(FaceName f);
		FaceName getActualFace(FaceName f);

		//to change face side
		//first parameter for the face in cube , second that face side changed to
		
		void setSide(FaceName f, FaceName x);

		
		void setFace(Face f, FaceName x);

		string fromColorToEdge(Color c1, Color c2);
		string fromColorToCorner(Color c1, Color c2, Color c3);

		Cubie toCubie(string s);

		short getOrientation(string s);

		// this is used after reading
		Cubies toCubiesFromSides();
		static Cubies CubeFromMovesList(string* moveList, int len);
};

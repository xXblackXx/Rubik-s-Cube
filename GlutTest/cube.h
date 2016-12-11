#pragma once

#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <string>
#include "face.h"
#include "utility.h"
#include "Cubies.h"

class Cube
{
    public:
        Cube();
        virtual ~Cube();

        Cube(string s);
		void strToCube(string s);

		bool operator == (const Cube& c);
		bool operator != (const Cube& c);

		void toSolved();
		bool isSolved();

		Face getFace(FaceName f);
		FaceName getActualFace(FaceName f);
		char colorToFaceLetter(Color c);

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
    private:
        Face up, down, front, back, right, left;
		string order = "FUDBRL";
		char center[6];
};

#endif // CUBE_H

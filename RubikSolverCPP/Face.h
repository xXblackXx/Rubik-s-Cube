#pragma once
#include "stdafx.h"
#include <iostream>
#include "utility.h"

using namespace std;

class Face
{
	private:
		Color sq[9];
		FaceName side;
	public:
		Face()
		{}
		Face(Color sq[]);
		Face(string s);

		Color operator[](int idx);
		bool operator==(Face f);
		bool operator!=(Face f);

		void setSide(FaceName s);
		FaceName getSide();

		void setSq(Color c, int i);
		Color getSq(int i);

		void setColOfCenter(Color c);
		Color colorOfCenter();


		void fillWithColor(Color c);
		void setUsingFace(Face f);
};
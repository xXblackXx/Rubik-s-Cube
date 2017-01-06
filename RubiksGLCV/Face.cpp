#include "Face.h"

Face::~Face()
{
    //dtor
}

Face::Face(Color sq[])
{
	for(int i = 0; i < 9; i++)
		sq[i] = sq[i];
}


Face::Face(string s)
{
	for(int i = 0; i < 9; i++)
		sq[i] = charToCol(s[i]);
}

Color Face::operator[](int idx)
{
	return sq[idx];
}

bool Face::operator==(Face _f)
{
	for (int i = 0; i < 9; i++)
		if (sq[i] != _f[i])
			return false;
	return true;
}
bool Face::operator!=(Face _f)
{
	for (int i = 0; i < 9; i++)
		if (sq[i] != _f[i])
			return true;
	return false;
}

void Face::setSide(FaceName s)
{
	side = s;
}

FaceName Face::getSide()
{
	return side;
}

void Face::setSq(Color c, int i)
{
	sq[i] = c;
}

Color Face::getSq(int i)
{
	return sq[i];
}


void Face::setColOfCenter(Color c)
{
	sq[4] = c;
}


Color Face::colorOfCenter()
{
	return sq[4];
}

void Face::fillWithColor(Color c)
{
	for(int i = 0; i < 9; i++)
		sq[i] = c;
}


void Face::setUsingFace(Face f)
{
	for(int i = 0; i < 9; i++)
		sq[i] = f.getSq(i);
}

#pragma once
#include "stdafx.h"
#include "utility.h"
#include "moves.h"

#include <vector>
#include <memory>

class Cubies
{
	private:
		short positions[20];
		short orientations[20];
		short CurrentStage = 0;

		Move moveGen;
		vector<shared_ptr<Cubies> > NextStates;
	public:
		Cubies();

		bool operator==(Cubies& cbs);

		void clockCycle(moves m);
		void counterCycle(moves m);
		short getPosition(int idx);
		short getOrientation(int idx);
		void setPosition(int idx, short val);
		void setOrientation(int idx, short val);
		bool isSolved();
		int Heuristic();
		bool HasNext();
		void resetNext();
		shared_ptr<Cubies> NextState();
		void GenerateNextStates();
		static shared_ptr<Cubies> Copy(shared_ptr<Cubies> b);
		static shared_ptr<Cubies> Copy(Cubies* b);
		static shared_ptr<Cubies> Copy(Cubies& b);
};
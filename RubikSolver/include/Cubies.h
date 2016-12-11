#pragma once

#ifndef CUBIES_H
#define CUBIES_H

#include "utility.h"

#include <vector>
#include <memory>
#include <math.h>
#include <algorithm>

class Cubies
{
    public:
        Cubies();
        virtual ~Cubies();

        int statesCount = 0;
		//shared_ptr<Cubies> NextStates[20];
		short positions[20];
		short orientations[20];

		bool operator==(Cubies& cbs);
		short lastOp;
		//vector<short> Solution;

		void clockCycle(moves m);
		void counterCycle(moves m);
		bool isSolved();
		Byte Heuristic();
		bool HasNext();
		void resetNext();
		//shared_ptr<Cubies> NextState();
		void GenerateNextStates(shared_ptr<Cubies>* NextStates);

		void SetPatternTables(Byte* CORNER_TABLE, Byte* EDGE1_TABLE, Byte* EDGE2_TABLE);


		static shared_ptr<Cubies> Copy(shared_ptr<Cubies> b);
		static shared_ptr<Cubies> Copy(Cubies* b);
		static shared_ptr<Cubies> Copy(Cubies b);
		int GetCornerHash();
		int GetEdge1Hash();
		int GetEdge2Hash();
    protected:
    private:
        short HeuristicValue;
		//short CurrentStage = 0;

        Byte CornerTableLookup(int hash);
		Byte Edge1TableLookup(int hash);
		Byte Edge2TableLookup(int hash);

		Byte* PtrCornerTable;
		Byte* PtrEdge1Table;
		Byte* PtrEdge2Table;

		//Move moveGen;

		void GenerateClockMoves();
		void GenerateCounterMoves();
		void GenerateHalfTurns();
};

#endif // CUBIES_H

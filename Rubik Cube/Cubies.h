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


            // functions to transform Cubies to Cube class

            // orientation to fix the cubie name
            // for edge normal swap would be sufficient
            // for corner rotate right or left
            string cornerWithOrientation(string c, short orient);
            string toStringOfFaces();

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
		void GenerateNextStates(Cubies* NextStates[]);

		void SetPatternTables(Byte* CORNER_TABLE, Byte* EDGE1_TABLE, Byte* EDGE2_TABLE);


		//static Cubies Copy(Cubies* b);
		static Cubies* Copy(Cubies* b);
		static Cubies Copy(Cubies b);
		int GetCornerHash();
		int GetEdge1Hash();
		int GetEdge2Hash();
    protected:
    private:
        int HeuristicValue;
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

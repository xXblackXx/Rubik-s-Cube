#include <iostream>
#include "PatternDatabase.h"
#include "include\Utility.h"
#include "Cubies.h"
#include "cube.h"
#include "RubikSolver.h"
#include "TableGenerator.h"

using namespace std;

void printSolution(short* path)
{
    const short *t = path;
    char face = '\0';
    while (*t != -1) {
        switch (*t) {
            case 0:
            case 6:
            case 12:
                face = 'U';
                break;
            case 1:
            case 7:
            case 13:
                face = 'F';
                break;
            case 2:
            case 8:
            case 14:
                face = 'D';
                break;
            case 3:
            case 9:
            case 15:
                face = 'B';
                break;
            case 4:
            case 10:
            case 16:
                face = 'R';
                break;
            case 5:
            case 11:
            case 17:
                face = 'L';
                break;
        }
        if (*t >= 12) {
            fprintf(stdout, "2%c ", face);
        } else if (*t >= 6) {
            fprintf(stdout, "%c' ", face);
        } else {
            fprintf(stdout, "%c ", face);
        }
        ++t;
    }
}


int main()
{
    /*
    TableGenerator tg;
    tg.EdgeTableGenerator(EDGE1_TABLE, true);
    FILE *output;
    output = fopen("table_edge1.mer", "w");
    int written;
    written = fwrite(EDGE1_TABLE, 1, 21288960, output);
    fclose(output);
    cout << written << endl;
    return 0;*/


    /*
    TableGenerator tg;
    tg.CornerTableGenerator(CORNER_TABLE);
    FILE *output;
    output = fopen("table_corner.mer", "w");
    int written;
    written = fwrite(CORNER_TABLE, 1, 44089920, output);
    fclose(output);
    cout << written << endl;
    return 0;*/


    InitPatternTables(CORNER_TABLE, EDGE1_TABLE, EDGE2_TABLE);
    LoadPatternTables(CORNER_TABLE, EDGE1_TABLE, EDGE2_TABLE);

    // 21 Q MOVES
    //string movs[] = {"U'", "U'", "B'", "B'", "D", "B'", "U'", "L'", "F'", "R'", "B", "L", "B", "R", "F", "L'", "F", "L", "U", "B'", "R'"};
    // 14 MOVES
    //string movs[] =  {"D", "U'", "R2", "B2", "F'", "L", "D", "U", "B", "F", "L", "U", "L2", "U'"};
    // 14 MOVES
    //string movs[] = {"B'", "L", "R2", "D'", "F", "L'", "B2", "L'", "R2", "D", "U2", "B2", "F", "R2"};
    // 14 MOVES
    string movs[] = {"U'", "L'", "R'", "B", "F'", "D'", "U", "L", "R'", "U", "L2", "F2", "L2", "B2" };
    // 12 MOVES
    //string movs[] = {"F2", "R", "B", "U", "F", "U", "L'", "B2", "F2", "L2", "D2", "R2"};
    // 10 MOVES
    //string movs[] = { "D", "B2", "F'", "L'", "D", "U2", "B2", "F2", "L2", "R'"};
    // 5 MOVES
    //string movs[] = { "D'", "U'", "R", "L'", "D'" };

    //string cube = "rbwyroybgowrwwowobgbogogorrbyyyywgbywwrggrwyybogrbgbro";
    //Cube c = Cube(cube);
	//shared_ptr<Cubies> cbs = Cubies::Copy(c.toCubiesFromSides());

    shared_ptr<Cubies> cbs = Cubies::Copy(Cube::CubeFromMovesList(movs, 14));
	cbs->SetPatternTables(CORNER_TABLE, EDGE1_TABLE, EDGE2_TABLE);

    for (int i = 0; i < 20; i++) cout << cbs->positions[i] << " \n"[i == 19];
    for (int i = 0; i < 20; i++) cout << cbs->orientations[i] << " \n"[i == 19];

    cout << cbs->GetCornerHash() << endl;
	cout << cbs->GetEdge1Hash() << endl;
	cout << cbs->GetEdge2Hash() << endl;
	cout << (int)cbs->Heuristic() << endl;

	RubikSolver solver(cbs);

	solver.Solve();

	printSolution(solver.actionLog);

	delete[] CORNER_TABLE;
	delete[] EDGE1_TABLE;
	delete[] EDGE2_TABLE;
    return 0;
}

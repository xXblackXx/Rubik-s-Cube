#ifndef TABLEGENERATOR_H
#define TABLEGENERATOR_H

#include "Cube.h"
#include "Cubies.h"
#include "Utility.h"
#include "Stack.h"

#include <vector>
#include <stack>
#include <math.h>
#include <algorithm>
#include <memory>

class TableGenerator
{
    public:
        void CornerTableGenerator(Byte* CORNER_TABLE);
        void EdgeTableGenerator(Byte* CORNER_TABLE, bool EdgeSet);

        TableGenerator();
        virtual ~TableGenerator();
    private:
};

#endif // TABLEGENERATOR_H

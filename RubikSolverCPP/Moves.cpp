#include "stdafx.h"
#include "Moves.h"

moves Move::opposite(moves m)
{
	if(int(m)%2 == 0)
		return (moves)(m + 1);
	return (moves)(m - 1);
}
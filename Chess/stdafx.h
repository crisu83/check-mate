// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

//Dissables the annoying warning: 
//warning C4146: unary minus operator applied to unsigned type, result still unsigned
//Which does not concern us.
#pragma warning(disable:4146)
//gets rid of warning C4018: '<=' : signed/unsigned mismatch warning
#pragma warning(disable:4018)
//gets rid of warning C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. warning
#pragma warning(disable:4996)

#pragma once

typedef unsigned long long UI64;

#define debug false

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <string>
#include <string.h>
#include <process.h>
#include <iostream>
#include <vector>

#include "Move.h"
#include "Position.h"
#include "Piece.h"
#include "Square.h"
#include "Board.h"
#include "Game.h"

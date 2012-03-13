#include "stdafx.h"

/**
	Main method.

	@author Christoffer Niska
	@param argc number of arguments
	@param argv argument values
	@return exit code
*/
int _tmain(int argc, _TCHAR* argv[])
{
	Game *game = new Game();
	return game->run();
}


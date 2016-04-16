#include <iostream>
#include <Game.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Red Ring Rico's Ludum Dare #35 Entry" << std::endl;

	LD::Game TheGame;

	if( TheGame.Initialise( ) != LD_OK )
	{
		std::cout << "[LD::main] <ERROR> Something went wrong initialising "
			"the game" << std::endl;

		return LD_FAIL;
	}

	return TheGame.Execute( );
}


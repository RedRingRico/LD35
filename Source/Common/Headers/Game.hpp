#ifndef __LD_GAME_HPP__
#define __LD_GAME_HPP__

#include <DataTypes.hpp>
#include <SDL2/SDL.h>
#include <Renderer.hpp>

namespace LD
{
	class Game
	{
	public:
		Game( );
		~Game( );

		LD_UINT32 Initialise( );

		LD_UINT32 Execute( );

	private:
		LD_UINT32 PlatformInitialise( );

		Renderer	m_Renderer;

		SDL_Window		*m_pWindow;
	};
}

#endif // __LD_GAME_HPP__


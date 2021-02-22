#include "Game.h"


void Game::Render()
{
	m_graphics->Render();
}

Game::Game(Window* window)
{
	ActiveWindow = window;

	m_graphics = new GraphicsManager(window->GetHWND());
}
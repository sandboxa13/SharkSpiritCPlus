#include "Game.h"


void Game::Render()
{
	m_ss_device->Render();
}

Game::Game(Window* window)
{
	ActiveWindow = window;

	m_ss_device = new GraphicsManager(window->GetHWND());
}
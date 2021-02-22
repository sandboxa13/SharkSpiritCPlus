#include "Game.h"


void Game::Render()
{
	m_graphics->Render();
}


Game::Game(Window* window)
{
	ActiveWindow = window;

	m_graphics = std::make_unique<GraphicsManager>(window->GetHWND());
}
#pragma once

#include "Window.h"
#include "GraphicsManager.h"

using namespace SSWindow;

class Game 
{
private:
	Window* ActiveWindow;
	GraphicsManager* m_graphics;
public:
	Game(Window* window);
	void Render();
};

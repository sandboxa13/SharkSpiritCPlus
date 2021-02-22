#pragma once

#include "Window.h"
#include "GraphicsManager.h"

using namespace SSWindow;

class Game 
{
private:
	Window* ActiveWindow;
	GraphicsManager* m_ss_device;
public:
	Game(Window* window);
	void Render();
};

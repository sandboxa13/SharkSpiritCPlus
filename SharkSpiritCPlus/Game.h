#pragma once

#include "Window.h"

using namespace SSWindow;

class Game 
{
private:
	Window* ActiveWindow;
public:
	Game(Window* window);
	void Render();
};

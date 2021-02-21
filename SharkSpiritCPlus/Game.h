#pragma once

#include "Window.h"
#include "SSDevice.h"

using namespace SSWindow;

class Game 
{
private:
	Window* ActiveWindow;
	SSDevice* m_ss_device;
public:
	Game(Window* window);
	void Render();
};

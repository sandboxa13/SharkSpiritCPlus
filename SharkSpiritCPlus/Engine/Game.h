#pragma once

#include "..//Core/Window.h"
#include "..//Render//GraphicsManager.h"
#include "memory.h"

using namespace SSWindow;

class Game 
{
private:
	Window* ActiveWindow;
	std::unique_ptr<GraphicsManager> m_graphics;
public:
	Game(Window* window);
	~Game() = default;
	void Render();
};

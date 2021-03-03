#include "Core//Window.h"
#include "Engine//Game.h"
#include <string>

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE prevInstance, 
	_In_ LPWSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	auto const window = new SSWindow::Window("Shark Spirit", "Shark Spirit", 720, 1280, hInstance);
	auto game = std::make_unique<Game>(window);

	auto wndProc = [&window, &game](UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT 
	{
		switch (msg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
			game->Render();
			ValidateRect(window->GetHWND(), nullptr);
			break;
		case WM_MOUSEMOVE:
			auto x = GET_X_LPARAM(lParam);
			auto y = GET_Y_LPARAM(lParam);

			std::string str;

			str = str + std::to_string(x);
			str = str + " ";
			str = str + std::to_string(y);

			SetWindowText(window->GetHWND(), str.c_str());
			break;
		}

		return DefWindowProc(window->GetHWND(), msg, wParam, lParam);
	};

	window->SetWndProc(wndProc);

	ShowWindow(window->GetHWND(), SW_SHOW);

	MSG msg = { nullptr };

	while (true)
	{
		if (PeekMessageW(&msg, window->GetHWND(), 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) { 
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		InvalidateRect(window->GetHWND(), nullptr, false);
	}

	game.reset();

	return msg.wParam;
}

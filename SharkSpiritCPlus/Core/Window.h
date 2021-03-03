#pragma once

#include <Windows.h>
#include <Windowsx.h>
#include <functional>

namespace SSWindow
{
	using WndProcCallBack = std::function<LRESULT(UINT, WPARAM, LPARAM)>;

	class Window
	{
	public:
		Window(const char* title, const  char* class_name, int height, int width, HINSTANCE& hinstance);
		~Window();

		void SetWndProc(WndProcCallBack callback);
		HWND GetHWND();
		LRESULT CALLBACK WndProc(_In_ HWND hwnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	private:
		int height;
		int width;
		const char* title;
		const char* className;
		HINSTANCE m_hinstance;
		HWND m_window_handle;
		WndProcCallBack m_wndproc_callback;
	};
}
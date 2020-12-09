#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <utilapiset.h>


bool toggled = false;
bool running = false;
bool hold = false;
std::mutex mutex;

void toggle_click(bool clicked)
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;

	if (!clicked)
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	else
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(input));
	ZeroMemory(&input, sizeof(input));
}

void click()
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;

	
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(input));
	ZeroMemory(&input, sizeof(input));

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	ZeroMemory(&input, sizeof(input));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	mutex.lock();
	running = false;
	mutex.unlock();
}

//int main()
//{
//	HWND console = GetConsoleWindow();
//	RECT ConsoleRect;
//	GetWindowRect(console, &ConsoleRect);
//	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 200, 20, TRUE);
//
//
//	
//}

int WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR    lpCmdLine,
	INT       cmdShow)
{
	for (;;)
	{
		if (GetAsyncKeyState(VK_XBUTTON2) == -32767)
		{
			toggled = !toggled;
		}
		else if (GetAsyncKeyState(VK_XBUTTON1) == -32767)
		{
			toggle_click(hold);
			hold = !hold;
		}

		if (toggled && !running && !hold)
		{
			running = true;
			std::thread thread(click);
			thread.detach();
		}



	}
}
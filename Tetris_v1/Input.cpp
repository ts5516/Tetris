#include "Input.h"

KEYCODE Input::getInput()
{
	if (clock() - inputTime >= 120)
	{
		inputTime = clock();
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			return KEYCODE::DOWN;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			return KEYCODE::LEFT;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			return KEYCODE::RIGHT;
		}
	}

	if (GetAsyncKeyState(VK_UP) & 0x0001)
	{
		return KEYCODE::UP;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
		return KEYCODE::SPACE;
	}
	if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
	{
		return KEYCODE::ESC;
	}
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		return KEYCODE::ENTER;
	}
	else
	{
		return KEYCODE::NOINPUT;
	}
}
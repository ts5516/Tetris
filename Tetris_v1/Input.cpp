#include "Input.h"

KEYCODE Input::getInput()
{
	if (GetAsyncKeyState(VK_UP) & 0x0001)
	{
		return KEYCODE::UP;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x0001)
	{
		return KEYCODE::DOWN;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x0001)
	{
		return KEYCODE::LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
	{
		return KEYCODE::RIGHT;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
		return KEYCODE::SPACE;
	}
	if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
	{
		return KEYCODE::ESC;
	}
	else
	{
		return KEYCODE::NOINPUT;
	}
}
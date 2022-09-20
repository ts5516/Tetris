#pragma once
#include <Windows.h>
#ifndef INPUT_H_
#define INPUT_H_

enum KEYCODE
{
	NOINPUT,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPACE,
	ESC
};

class Input
{
public:
	Input() {};

	KEYCODE getInput();
private:
};
#endif // !INPUT_H_

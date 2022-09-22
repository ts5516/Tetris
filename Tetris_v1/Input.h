#pragma once
#include <Windows.h>
#include <ctime>
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
	ENTER,
	ESC
};

class Input
{
public:
	Input()
	:inputTime(clock()) {};

	KEYCODE getInput();
private:
	clock_t inputTime;
};
#endif // !INPUT_H_

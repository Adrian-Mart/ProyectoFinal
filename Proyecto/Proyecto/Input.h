#pragma once
#include "Window.h"
#include <list>
class Input
{
public:
	Input(Window window);
	void Update();
	void AddKey(int key);
	~Input();
private:
	Window window;
	std::list<int> keys;
};


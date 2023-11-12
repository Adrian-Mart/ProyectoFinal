#pragma once
#include "Window.h"

class Key
{
public:
	Key();
	void Update(bool keyDown);
	~Key();
	bool IsPressed() { return pressed; }
	bool IsReleased() { return released; }
	bool IsHold() { return hold; }
private:
	bool hold;
	bool pressed;
	bool released;
};


#include "Key.h"

Key::Key()
{
	hold = false;
	pressed = false;
	released = false;
}

void Key::Update(bool keyDown)
{
	if (keyDown)
	{
		if (!hold)
		{
			pressed = true;
			hold = true;
		}
		else
		{
			pressed = false;
		}
		released = false;
	}
	else
	{
		if (hold)
		{
			released = true;
			hold = false;
		}
		else
		{
			released = false;
		}
		pressed = false;
	}
}

Key::~Key(){}

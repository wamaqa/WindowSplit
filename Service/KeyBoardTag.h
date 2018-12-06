// 2018112922:34

#pragma once
#define KeyUp 1
#define KeyDown 2
#define KeyNone 0

enum KeyState
{
	None = 0,
	Down = 1,
	Up = 2
};

struct KeyBoardTag
{
	KeyState LCTRL;
	DWORD LCTRLTime;

	KeyBoardTag()
	{
		LCTRL = None;
		LCTRLTime = 0;
	}
};



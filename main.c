#include "memcheck.h"

#include "keyboard.h"

#include <stdio.h>

int allocCount = 0;

int main()
{
	KBManager* kbMgr = multiKBSetup();

	bool running = true;

	while (running)
	{
		multiKBUpdate(kbMgr);
		if (kbMgr->numKB > 1)
			if (kbMgr->kb[1].keys[key_Escape])
				break;
	}

	multiKBShutdown(kbMgr);

	return 0;
}
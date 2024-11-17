#include "keyboard.h"

#include <stdio.h>

int main()
{
	KBManager kbMgr;
	multiKBSetup(&kbMgr);

	bool running = true;

	while (running)
	{
		multiKBUpdate(&kbMgr);
		for (uint32_t i = 0; i < kbMgr.numKB; i++)
			if (kbMgr.kb[i]->keys[key_Escape])
				running = false;
	}

	multiKBShutdown(&kbMgr);

	return 0;
}
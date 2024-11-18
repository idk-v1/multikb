#include "keyboard.h"

#include <stdio.h>

int main()
{
	KBManager kbMgr;
	kbMgr.useToggle = true;
	multiKB_Setup(&kbMgr);

	bool running = true;

	while (running)
	{
		multiKB_Update(&kbMgr);
		for (uint32_t i = 0; i < kbMgr.numKB; i++)
			if (multiKB_Key(&kbMgr, i, key_Escape))
				running = false;

		printf("%u %u %u\n",
			(kbMgr.kb[1]->keys[key_Capslock] >> 0) & 1,
			(kbMgr.kb[1]->keys[key_Capslock] >> 1) & 1,
			(kbMgr.kb[1]->keys[key_Capslock] >> 2) & 1);
	}

	multiKB_Shutdown(&kbMgr);

	return 0;
}
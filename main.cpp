#include "keyboard.h"

int main()
{
	KBManager kbMgr;
	multiKB_Setup(&kbMgr);

	bool running = true;
	while (running)
	{
		multiKB_Update(&kbMgr);
		for (uint32_t i = 0; i < kbMgr.numKB; i++)
			if (multiKB_Key(&kbMgr, i, key_Escape))
				running = false;
	}

	multiKB_Shutdown(&kbMgr);
	return 0;
}

int main()
{
	KBManagerPP kbMgr;

	bool running = true;
	while (running)
	{
		kbMgr.update();
		for (uint32_t i = 0; i < kbMgr.getNumDevices(); i++)
			if (kbMgr.key(i, key_Escape))
				running = false;
	}

	return 0;
}
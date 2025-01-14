#include "multikb.h"
#include <stdio.h>
#include <iostream>

int main()
{
	KBManager kbMgr;
	multiKB_Setup(&kbMgr);

	for (uint32_t i = 0; i < kbMgr.numKB; i++)
		printf("%s: %u keys\n", multiKB_GetDeviceName(&kbMgr, i), 
			multiKB_GetDeviceKeyCount(&kbMgr, i));

	bool running = true;
	while (running)
	{
		multiKB_Update(&kbMgr);
		for (uint32_t i = 0; i < kbMgr.numKB; i++)
			if (multiKB_Key(&kbMgr, i, key_Escape))
			{
				printf("\nEscape Pressed: Keyboard %d %s\n", 
					i, multiKB_GetDeviceName(&kbMgr, i));
				running = false;
			}
	}

	multiKB_Shutdown(&kbMgr);
	return 0;
}
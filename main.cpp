#include "multikb.h"
#include <stdio.h>
#include <iostream>

int main()
{
	KBManager kbMgr;
	multiKB_Setup(&kbMgr);

	for (uint32_t i = 0; i < kbMgr.numKB; i++)
		printf("%s\n", multiKB_GetDeviceName(&kbMgr, i));

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

int main2()
{
	KBManagerPP kbMgr;

	for (uint32_t i = 0; i < kbMgr.getNumDevices(); i++)
		std::cout << kbMgr.getDeviceName(i) << std::endl;

	bool running = true;
	while (running)
	{
		kbMgr.update();
		for (uint32_t i = 0; i < kbMgr.getNumDevices(); i++)
			if (kbMgr.key(i, key_Escape))
			{
				std::cout << std::endl << "Escape Pressed: Keyboard " 
					<< i << kbMgr.getDeviceName(i) << std::endl;
				running = false;
			}
	}

	return 0;
}
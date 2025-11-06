#include <stdio.h>

#include "multikb.h"

#define PRINTBOOL(cond) ((cond) ? "true" : "false")

int main()
{
	bool running = mkb_init();

	printf("Devices: %llu\n", mkb_deviceConnectedCount());
	for (int i = 0; i < mkb_deviceCount(); i++)
	{
		if (mkb_deviceState(i))
			printf("[%d]: \"%s\"\n", i, mkb_deviceName(i));
	}

	while (running)
	{
		mkb_update();
		for (uint64_t i = 0; i < mkb_deviceCount(); i++)
		{
			switch (mkb_getDeviceEvent(i))
			{
			case mkb_DEVICE_CONNECT:
				printf("Device %llu Connected: \"%s\"\n", i, mkb_deviceName(i));
				break;

			case mkb_DEVICE_RECONNECT:
				printf("Device %llu Reconnected: \"%s\"\n", i, mkb_deviceName(i));
				break;

			case mkb_DEVICE_DISCONNECT:
				printf("Device %llu Disconnected: \"%s\"\n", i, mkb_deviceName(i));
				break;
			}
		}

		for (int i = 0; i < mkb_deviceCount(); i++)
		{
			if (mkb_deviceState(i))
			{
				uint8_t key = mkb_lastKey(i);
				if (key)
				{
					if (mkb_keyDown(i, key))
					{
						printf("Keyboard [%d]: Pressed %s\n", i, mkb_keyNames[key]);
						switch (key)
						{
						case mkb_KEY_CAPSLOCK:   
							printf("%s State: %s\n", 
								mkb_keyNames[key], 
								PRINTBOOL(mkb_capslockState()));   
							break;
						case mkb_KEY_NUMLOCK:    
							printf("%s State: %s\n", 
								mkb_keyNames[key], 
								PRINTBOOL(mkb_numlockState()));    
							break;
						case mkb_KEY_SCROLLLOCK: 
							printf("%s State: %s\n",
								mkb_keyNames[key], 
								PRINTBOOL(mkb_scrolllockState())); 
							break;
						}
					}
					if (mkb_keyUp(i, key))
						printf("Keyboard [%d]: Released %s\n", i, mkb_keyNames[key]);
				}
			}
		}

		for (int i = 0; i < mkb_deviceCount(); i++)
			if (mkb_keyState(i, mkb_KEY_ESC))
				running = false;
	}

	mkb_shutdown();

	return 0;
}
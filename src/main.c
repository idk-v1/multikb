#include <stdio.h>

#include "multikb.h"

int main()
{
	bool running = mkb_init();

	printf("Devices: %llu\n", mkb_deviceConnectedCount());

	while (running)
	{
		mkb_update();
		switch (mkb_getLastEvent())
		{
		case mkb_DEVICE_CONNECT:
			printf("Device %llu Connected\n", mkb_getLatestDevice());
			break;

		case mkb_DEVICE_RECONNECT:
			printf("Device %llu Reconnected\n", mkb_getLatestDevice());
			break;

		case mkb_DEVICE_DISCONNECT:
			printf("Device %llu Disconnected\n", mkb_getLatestDevice());
			break;
		}

		for (int i = 0; i < mkb_deviceCount(); i++)
		{
			for (int k = 0; k < mkb_KEY_COUNT; k++)
			{
				if (mkb_keyDown(i, k))
				{
					printf("Keyboard [%d]: Pressed %s\n", i, mkb_keyNames[k]);
					if (k == mkb_KEY_CAPSLOCK)
						printf("Capslock State: %s\n", (mkb_capslockState()) ? "true" : "false");
					else if (k == mkb_KEY_NUMLOCK)
						printf("Numlock State: %s\n", (mkb_numlockState()) ? "true" : "false");
				}
				if (mkb_keyUp(i, k))
					printf("Keyboard [%d]: Released %s\n", i, mkb_keyNames[k]);
			}
		}

		for (int i = 0; i < mkb_deviceCount(); i++)
			if (mkb_keyState(i, mkb_KEY_ESC))
				running = false;
	}

	mkb_shutdown();

	return 0;
}
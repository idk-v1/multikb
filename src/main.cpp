#include <iostream>

#include "multikb.h"

int main()
{
	MultiKB mkb;
	bool running = true;

	std::cout << "Devices: " << mkb.deviceConnectedCount() << std::endl;

	while (running)
	{
		mkb.update();
		switch (mkb.getLastEvent())
		{
		case mkb_DEVICE_CONNECT:
			std::cout << "Device " << mkb.getLatestDevice() << "Connected" << std::endl;
			break;

		case mkb_DEVICE_RECONNECT:
			std::cout << "Device " << mkb.getLatestDevice() << "Reconnected" << std::endl;
			break;

		case mkb_DEVICE_DISCONNECT:
			std::cout << "Device " << mkb.getLatestDevice() << "Disconnected" << std::endl;
			break;
		}

		for (int i = 0; i < mkb.deviceCount(); i++)
		{
			for (int k = 0; k < mkb_KEY_COUNT; k++)
			{
				if (mkb.keyDown(i, k))
				{
					std::cout << "Keyboard [" << i << "]: Pressed " << mkb_keyNames[k] << std::endl;
					if (k == mkb_KEY_CAPSLOCK)
						std::cout << "Capslock State: " << (mkb.capslockState() ? "true" : "false") << std::endl;
					else if (k == mkb_KEY_NUMLOCK)
						std::cout << "Numlock State: " << (mkb.numlockState() ? "true" : "false") << std::endl;
				}
				if (mkb.keyUp(i, k))
					std::cout << "Keyboard [" << i << "]: Released " << mkb_keyNames[k] << std::endl;
			}
		}

		for (int i = 0; i < mkb.deviceCount(); i++)
			if (mkb.keyState(i, mkb_KEY_ESC))
				running = false;
	}

	return 0;
}
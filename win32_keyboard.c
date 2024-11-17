#ifdef __cplusplus
extern "C" {
#endif

//#include "memcheck.h"

#define WIN32_LEAN_AND_MEAN
#define NO_STRICT
#define NOMINMAX
#include <Windows.h>
#include <hidusage.h>
#include <hidsdi.h>
#pragma comment(lib, "hid.lib")

#include "keyboard.h"

	struct _OSKBInfo
	{
		void* msgWindow;
		void** devHndl;
	};

	static char* getDeviceName(void* hndl)
	{
		uint32_t size = 0;
		char* name = NULL;

		GetRawInputDeviceInfoA(hndl, RIDI_DEVICENAME, NULL, &size);
		if (size == 0)
			return NULL;

		name = malloc(size);
		GetRawInputDeviceInfoA(hndl, RIDI_DEVICENAME, name, &size);
		return name;
	}

	static uint32_t findDeviceFromName(char* name, KBManager* kbMgr)
	{
		for (uint32_t i = 0; i < kbMgr->numKB; i++)
			if (kbMgr->kb[i]->state == 0)
				if (strcmp(name, kbMgr->kb[i]->devName) == 0)
					return i;

		return -1;
	}

	static uint32_t deviceIDFromHndl(void* hndl, KBManager* kbMgr)
	{
		for (uint32_t i = 0; i < kbMgr->numKB; i++)
			if (kbMgr->_osInfo->devHndl[i] == hndl)
				return i;
		return -1;
	}

	static void addDevice(void* hndl, KBManager* kbMgr)
	{
		char* name = getDeviceName(hndl);
		uint32_t index = findDeviceFromName(name, kbMgr);

		if (index == -1)
		{
			void* ptr = realloc(kbMgr->kb, sizeof(void*) * (kbMgr->numKB + 1));
			if (ptr)
			{
				kbMgr->kb = ptr;

				kbMgr->kb[kbMgr->numKB] = malloc(sizeof(Keyboard));
				memset(kbMgr->kb[kbMgr->numKB], 0, sizeof(Keyboard));
				kbMgr->kb[kbMgr->numKB]->state = 1;
				kbMgr->kb[kbMgr->numKB]->devName = name;

				void* hndlPtr = realloc(kbMgr->_osInfo->devHndl, sizeof(void*) * (kbMgr->numKB + 1));
				if (hndlPtr)
				{
					printf("Added device [%u]\n", kbMgr->numKB);

					kbMgr->_osInfo->devHndl = hndlPtr;
					kbMgr->_osInfo->devHndl[kbMgr->numKB] = hndl;

					kbMgr->numKB++;
					return;
				}
			}
			multiKBShutdown(kbMgr);
		}
		else
		{
			printf("Readded device [%u]\n", index);
			kbMgr->_osInfo->devHndl[index] = hndl;
			kbMgr->kb[index]->state = 1;
			free(name);
		}
	}

	static void removeDevice(void* hndl, KBManager* kbMgr)
	{
		uint32_t index = deviceIDFromHndl(hndl, kbMgr);
		if (index != -1)
		{
			memset(kbMgr->kb[index]->keys, 0, key_Count);
			kbMgr->kb[index]->state = 0;
			printf("Removed device [%u]\n", index);
		}
	}

	static uint32_t keyFromRaw(RAWKEYBOARD key)
	{
		if (key.VKey >= 'A' && key.VKey <= 'Z')
			return key.VKey - 'A' + key_A;
		if (key.VKey >= '0' && key.VKey <= '9')
			return key.VKey - '0' + key_0;
		if (key.VKey >= '0' + 48 && key.VKey <= '9' + 48)
			return key.VKey - '0' - 48 + key_Num0;
		if (key.VKey >= 112 && key.VKey <= 124)
			return key.VKey - 112 + key_Fn1;

		switch (key.VKey)
		{
		case  20: return key_Capslock;
		case 144: return key_Numlock;

		case  17: return key_Control;
		case  16: return key_Shift;
		case  18: return key_Alt;
		case  91: 
		case  92: return key_System;
		case  27: return key_Escape;
		case   8: return key_BackSp;
		case   9: return key_Tab;
		case  13: return key_Enter;
		case  46: return key_Delete;
		case  35: return key_End;
		case  36: return key_Home;
		case  45: return key_Insert;
		case  44: return key_PrintScr;
		case  33: return key_PageUp;
		case  34: return key_PageDown;
		case  37: return key_Left;
		case  38: return key_Up;
		case  39: return key_Right;
		case  40: return key_Down;

		case  32: return key_Space;
		case 222: return key_Apostr;
		case 188: return key_Comma;
		case 189: return key_Minus;
		case 190: return key_Period;
		case 191: return key_FSlash;

		case 186: return key_Semicolon;
		case 187: return key_Equal;

		case 219: return key_LBracket;
		case 220: return key_BSlash;
		case 221: return key_RBracket;

		case 192: return key_Grave;

		case 111: return key_NumDiv;
		case 106: return key_NumMult;
		case 109: return key_NumSub;
		case 107: return key_NumAdd;
		//case : return key_NumEnter;
		case 110: return key_NumPeriod;

		case 255: return 0;
		}

		printf("unknown: %u\n", key.VKey);
		return 0; // Unnown
	}

	static void parseInput(void* hndl, KBManager* kbMgr)
	{
		RAWINPUT data;
		uint32_t size = sizeof(data);
		GetRawInputData(hndl, RID_INPUT, &data, &size, sizeof(RAWINPUTHEADER));

		uint32_t key = keyFromRaw(data.data.keyboard);
		uint32_t device = deviceIDFromHndl(data.header.hDevice, kbMgr);
		bool state = (data.data.keyboard.Message == WM_KEYDOWN);

		if (key != 0)
		{
			if (state != kbMgr->kb[device]->keys[key])
				printf("Device [%u]: %s %s\n", device, keyNames[key], state ? "Pressed" : "Released");
			kbMgr->kb[device]->keys[key] = state;
		}
	}

	static uint64_t __stdcall multiKBProc(void* wnd, uint32_t msg, uint64_t data1, uint64_t data2)
	{
		KBManager* kbMgr = GetWindowLongPtrA(wnd, GWLP_USERDATA);
		if (kbMgr == NULL)
			return DefWindowProcA(wnd, msg, data1, data2);

		switch (msg)
		{
		case WM_INPUT:
			parseInput(data2, kbMgr);
			return 0;

		case WM_INPUT_DEVICE_CHANGE:
			if (data1 == GIDC_ARRIVAL)
				addDevice(data2, kbMgr);
			else if (data1 == GIDC_REMOVAL)
				removeDevice(data2, kbMgr);

			return 0;

		default:
			return DefWindowProcA(wnd, msg, data1, data2);
		}
	}

	static void* hiddenWindow(KBManager* kbMgr)
	{
		void* inst = GetModuleHandleA(NULL);

		WNDCLASSA wc = { 0 };
		wc.hInstance = inst;
		wc.lpfnWndProc = multiKBProc;
		wc.lpszClassName = "multiKBMsg";
		RegisterClassA(&wc);

		void* wnd = CreateWindowExA(0, "multiKBMsg", "", 0, 0, 0, 0, 0, 0, 0, inst, 0);
		if (wnd)
			SetWindowLongPtrA(wnd, GWLP_USERDATA, kbMgr);

		return wnd;
	}


	bool multiKBSetup(KBManager* kbMgr)
	{
		kbMgr->kb = NULL;
		kbMgr->numKB = 0;

		kbMgr->_osInfo = malloc(sizeof(_OSKBInfo));
		if (kbMgr->_osInfo)
		{
			kbMgr->_osInfo->devHndl = NULL;

			kbMgr->_osInfo->msgWindow = hiddenWindow(kbMgr);
			if (kbMgr->_osInfo->msgWindow)
			{
				RAWINPUTDEVICE devType;
				// Generic
				devType.usUsagePage = 1; 
				// Keyboard
				devType.usUsage = 6; 
				// device add/remove & messages if any of this program's windows are focused
				devType.dwFlags = RIDEV_DEVNOTIFY | RIDEV_EXINPUTSINK; 
				devType.hwndTarget = kbMgr->_osInfo->msgWindow;

				if (RegisterRawInputDevices(&devType, 1, sizeof(devType)))
					return kbMgr;
			}
		}

		multiKBShutdown(kbMgr);
		return NULL;
	}

	void multiKBUpdate(KBManager* kbMgr)
	{
		MSG msg;
		while (PeekMessageA(&msg, kbMgr->_osInfo->msgWindow, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	void multiKBShutdown(KBManager* kbMgr)
	{
		if (kbMgr->_osInfo)
		{
			if (kbMgr->_osInfo->msgWindow)
			{
				DestroyWindow(kbMgr->_osInfo->msgWindow);
				UnregisterClassA("multiKBMsg", NULL);
			}
			if (kbMgr->_osInfo->devHndl)
				free(kbMgr->_osInfo->devHndl);
			free(kbMgr->_osInfo);
		}
		if (kbMgr->kb)
		{
			for (uint32_t i = 0; i < kbMgr->numKB; i++)
			{
				if (kbMgr->kb[i]->devName)
					free(kbMgr->kb[i]->devName);
				free(kbMgr->kb[i]);
			}
			free(kbMgr->kb);
		}
		kbMgr->kb = NULL;
		kbMgr->_osInfo = NULL;
		kbMgr->numKB = 0;
	}

#ifdef __cplusplus
}
#endif
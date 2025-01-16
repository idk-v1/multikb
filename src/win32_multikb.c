#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NO_STRICT
#pragma comment(lib, "hid.lib")
#include <Windows.h>
#include <hidusage.h>
#include <hidsdi.h>

#include "multikb.h"

static HWND msgWindow = NULL;
static HANDLE* devHndl = NULL;

uint64_t _mkb_keyboardNum = 0;
mkb_Keyboard** _mkb_keyboards = NULL;
uint64_t _mkb_latestDev = -1;


static inline char* getDeviceName(HANDLE hndl)
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

static inline RID_DEVICE_INFO_KEYBOARD getDeviceInfo(HANDLE hndl)
{
	RID_DEVICE_INFO info = { 0 };
	info.cbSize = sizeof(info);

	uint32_t size = sizeof(info);

	GetRawInputDeviceInfoA(hndl, RIDI_DEVICEINFO, &info, &size);
	return info.keyboard;
}


static inline uint64_t deviceIndexFromHandle(HANDLE hndl)
{
	for (uint64_t i = 0; i < mkb_deviceCount(); i++)
		if (devHndl[i] == hndl)
			return i;

	return -1;
}
static inline uint64_t deviceHandleFromName(const char* name)
{
	for (uint64_t i = 0; i < mkb_deviceCount(); i++)
		if (_mkb_keyboards[i]->connected == 0) // previously connected and much cheaper than strcmp
			if (strcmp(name, _mkb_keyboards[i]->name) == 0)
				return i;

	return -1;
}


static void mkb_addDevice(HANDLE hndl)
{
	uint64_t keyCount = getDeviceInfo(hndl).dwNumberOfKeysTotal;
	if (keyCount < 26) // not a real keyboard (or at least not useful), don't add
		return;

	char* name = getDeviceName(hndl);
	uint64_t index = deviceHandleFromName(name);

	// Create new device, couldn't find matching name
	if (index == -1)
	{
		_mkb_keyboardNum++;

		// resize
		void* devHndlPtr = realloc(devHndl, mkb_deviceCount() * sizeof(HANDLE));
		if (!devHndlPtr)
		{
			mkb_shutdown();
			return;
		}
		devHndl = devHndlPtr;

		void* keyboardPtr = realloc(_mkb_keyboards, mkb_deviceCount() * sizeof(mkb_Keyboard*));
		if (!keyboardPtr)
		{
			mkb_shutdown();
			return;
		}
		_mkb_keyboards = keyboardPtr;

		// alloc new keyboard
		mkb_Keyboard* kb = malloc(sizeof(mkb_Keyboard));
		if (kb)
		{
			memset(kb, 0, sizeof(mkb_Keyboard));
			kb->connected = true;
			kb->keyCount = (uint32_t)keyCount;
			kb->name = name;

			// add to array
			index = mkb_deviceCount() - 1;
			_mkb_keyboards[index] = kb;
			devHndl[index] = hndl;
		}
		else
			return;
	}
	else // found matching device name, reconnected
	{
		// update handle and change connected state
		devHndl[index] = hndl;
		_mkb_keyboards[index]->connected = true;

		// discard new name
		if (name)
			free(name);
	}

	_mkb_latestDev = index;
}

static void mkb_removeDevice(HANDLE hndl)
{
	uint64_t index = deviceIndexFromHandle(hndl);
	if (index != -1)
	{
		// set all key states to off,just in case
		memset(_mkb_keyboards[index]->keys, 0, mkb_KEY_COUNT);
		_mkb_keyboards[index]->connected = false;
		devHndl[index] = NULL;

		_mkb_latestDev = index;
	}
}

static uint64_t keyFromRaw(RAWKEYBOARD key)
{
	if (key.VKey >= 'A' && key.VKey <= 'Z')
		return key.VKey - 'A' + mkb_KEY_A;
	if (key.VKey >= '0' && key.VKey <= '9')
		return key.VKey - '0' + mkb_KEY_0;
	if (key.VKey >= '0' + 48 && key.VKey <= '9' + 48)
		return key.VKey - '0' - 48 + mkb_KEY_NUM0;
	if (key.VKey >= 112 && key.VKey <= 124)
		return key.VKey - 112 + mkb_KEY_FN1;

	switch (key.VKey)
	{
	case  20: return mkb_KEY_CAPSLOCK;
	case 144: return mkb_KEY_NUMLOCK;

	case  17: return mkb_KEY_CTRL;
	case  16: return mkb_KEY_SHIFT;
	case  18: return mkb_KEY_ALT;
	case  91:
	case  92: return mkb_KEY_SYS;
	case  27: return mkb_KEY_ESC;
	case   8: return mkb_KEY_BACKSP;
	case   9: return mkb_KEY_TAB;
	case  13: return mkb_KEY_ENTER;
	case  46: return mkb_KEY_DEL;
	case  35: return mkb_KEY_END;
	case  36: return mkb_KEY_HOME;
	case  45: return mkb_KEY_INSERT;
	case  44: return mkb_KEY_PRINTSCR;
	case  33: return mkb_KEY_PAGEUP;
	case  34: return mkb_KEY_PAGEDOWN;
	case  37: return mkb_KEY_LEFT;
	case  38: return mkb_KEY_UP;
	case  39: return mkb_KEY_RIGHT;
	case  40: return mkb_KEY_DOWN;

	case  32: return mkb_KEY_SPACE;
	case 222: return mkb_KEY_APOSTR;
	case 188: return mkb_KEY_COMMA;
	case 189: return mkb_KEY_MINUS;
	case 190: return mkb_KEY_PERIOD;
	case 191: return mkb_KEY_FSLASH;

	case 186: return mkb_KEY_SEMICOLON;
	case 187: return mkb_KEY_EQUAL;

	case 219: return mkb_KEY_LBRACK;
	case 220: return mkb_KEY_BSLASh;
	case 221: return mkb_KEY_RBRACK;

	case 192: return mkb_KEY_GRAVE;

	case 111: return mkb_KEY_NUMDIV;
	case 106: return mkb_KEY_NUMMUL;
	case 109: return mkb_KEY_NUMSUB;
	case 107: return mkb_KEY_NUMADD;
	case 110: return mkb_KEY_NUMPERIOD;

	case 255: return 0;
	}

	return 0; // Unnown
}

static void parseInput(HANDLE hndl)
{
	RAWINPUT data = { 0 };
	uint32_t size = sizeof(data);
	GetRawInputData(hndl, RID_INPUT, &data, &size, sizeof(RAWINPUTHEADER));

	uint64_t key = keyFromRaw(data.data.keyboard);
	uint64_t device = deviceIndexFromHandle(data.header.hDevice);
	if (key == 0 || device == -1)
		return;

	_mkb_keyboards[device]->keys[key].state = (data.data.keyboard.Message == WM_KEYDOWN);
}

static LRESULT WINAPI mkb_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INPUT:
		parseInput((HANDLE)lp);

		if (wp == RIM_INPUT)
			DefWindowProcA(hwnd, msg, wp, lp);
		return 0;

	case WM_INPUT_DEVICE_CHANGE:
		if (wp == GIDC_ARRIVAL)
			mkb_addDevice((HANDLE)lp);
		else if (wp == GIDC_REMOVAL)
			mkb_removeDevice((HANDLE)lp);

		return 0;

	default: return DefWindowProcA(hwnd, msg, wp, lp);
	}
}


bool mkb_init()
{
	// register window class
	WNDCLASSA wc = { 0 };
	wc.lpfnWndProc = mkb_wndproc;
	wc.hInstance = GetModuleHandleA(NULL);
	wc.lpszClassName = "multikeyboard";
	RegisterClassA(&wc);

	// create hidden window to recieve messages
	msgWindow = CreateWindowExA(0, "multikeyboard", "", 0, 0, 0, 0, 0, 
		NULL, NULL, GetModuleHandleA(NULL), NULL);

	// request keyboard devices
	RAWINPUTDEVICE devType = { 0 };
	devType.usUsagePage = 1; // Generic
	devType.usUsage = 6; // Keyboard
	devType.dwFlags = RIDEV_DEVNOTIFY | RIDEV_INPUTSINK; // device add/remove & messages in background
	devType.hwndTarget = msgWindow;
	RegisterRawInputDevices(&devType, 1, sizeof(devType));

	// update once to add devices
	mkb_update();
	return true;
}

uint8_t mkb_update()
{
	uint64_t count = mkb_deviceConnectedCount();
	uint64_t maxCount = mkb_deviceCount();

	// Update key states
	for (uint64_t dev = 0; dev < mkb_deviceCount(); dev++)
		for (uint64_t i = 0; i < mkb_KEY_COUNT; i++)
			_mkb_keyboards[dev]->keys[i].lastState =
				_mkb_keyboards[dev]->keys[i].state;

	MSG msg;
	while (PeekMessageA(&msg, msgWindow, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	uint64_t newCount = mkb_deviceConnectedCount();
	uint64_t newMaxCount = mkb_deviceCount();

	if (newCount > count)
	{
		if (newMaxCount > maxCount)
			return mkb_DEVICE_CONNECT;
		else
			return mkb_DEVICE_RECONNECT;
	}
	else if (newCount < count)
		return mkb_DEVICE_DISCONNECT;

	return mkb_DEVICE_NONE;
}

uint64_t mkb_deviceCount()
{
	return _mkb_keyboardNum;
}

 uint64_t mkb_deviceConnectedCount()
{
	uint64_t count = 0;
	for (uint64_t i = 0; i < mkb_deviceCount(); i++)
		if (_mkb_keyboards[i]->connected)
			count++;

	return count;
}

 uint64_t mkb_getLatestDevice()
 {
	 return _mkb_latestDev;
 }

const char* mkb_deviceName(uint64_t index)
{
	if (index < mkb_deviceCount())
		return _mkb_keyboards[index]->name;
	return NULL;
}

bool mkb_keyState(uint64_t index, uint8_t key)
{
	if (index < mkb_deviceCount())
		return _mkb_keyboards[index]->keys[key].state;
	return false;
}

bool mkb_keyLast(uint64_t index, uint8_t key)
{
	if (index < mkb_deviceCount())
		return _mkb_keyboards[index]->keys[key].lastState;
	return false;
}

bool mkb_keyDown(uint64_t index, uint8_t key)
{
	if (index < mkb_deviceCount())
		return (_mkb_keyboards[index]->keys[key].state && 
			!_mkb_keyboards[index]->keys[key].lastState);
	return false;
}

bool mkb_keyUp(uint64_t index, uint8_t key)
{
	if (index < mkb_deviceCount())
		return (!_mkb_keyboards[index]->keys[key].state &&
			_mkb_keyboards[index]->keys[key].lastState);
	return false;
}

void mkb_shutdown()
{
	DestroyWindow(msgWindow);
	UnregisterClassA("multikeyboard", GetModuleHandleA(NULL));

	free(devHndl);
	devHndl = NULL;

	for (uint64_t i = 0; i < mkb_deviceCount(); i++)
	{
#pragma warning (disable: 6001) // Bad visual studio
		if (_mkb_keyboards[i]->name)
			free(_mkb_keyboards[i]->name);
		if (_mkb_keyboards[i])
			free(_mkb_keyboards[i]);
	}
}

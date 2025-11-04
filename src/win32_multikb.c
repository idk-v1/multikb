#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NO_STRICT
#include <Windows.h>
#include <hidsdi.h>

#include "multikb.h"

static HWND msgWindow = NULL;
static HANDLE* devHndl = NULL;

uint64_t _mkb_keyboardNum = 0;
mkb_Keyboard** _mkb_keyboards = NULL;
uint64_t _mkb_latestDev = -1;

bool _mkb_isInit = false;
uint8_t _mkb_lastEvent = mkb_DEVICE_NONE;


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
			kb->keyCount = keyCount;
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
		_mkb_keyboards[index]->lastKey = 0;
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
	if (key.VKey >= VK_NUMPAD0 && key.VKey <= VK_NUMPAD9)
		return key.VKey - VK_NUMPAD0 + mkb_KEY_NUM0;
	if (key.VKey >= VK_F1 && key.VKey <= VK_F24)
		return key.VKey - VK_F1 + mkb_KEY_FN1;

	switch (key.VKey)
	{
	case VK_CAPITAL: return mkb_KEY_CAPSLOCK;
	case VK_NUMLOCK: return mkb_KEY_NUMLOCK;
	case VK_SCROLL: return mkb_KEY_SCROLLLOCK;

	case VK_ESCAPE: return mkb_KEY_ESC;
	case VK_BACK: return mkb_KEY_BACKSP;
	case VK_TAB: return mkb_KEY_TAB;
	case VK_RETURN: return ((key.Flags & RI_KEY_E0) ? mkb_KEY_NUMENTER : mkb_KEY_ENTER);
	case VK_DELETE: return mkb_KEY_DEL;
	case VK_END: return mkb_KEY_END;
	case VK_HOME: return mkb_KEY_HOME;
	case VK_INSERT: return mkb_KEY_INSERT;
	case VK_SNAPSHOT: return mkb_KEY_PRINTSCR;
	case VK_PRIOR: return mkb_KEY_PAGEUP;
	case VK_NEXT: return mkb_KEY_PAGEDOWN;
	case VK_LEFT: return mkb_KEY_LEFT;
	case VK_UP: return mkb_KEY_UP;
	case VK_RIGHT: return mkb_KEY_RIGHT;
	case VK_DOWN: return mkb_KEY_DOWN;

	case VK_CONTROL: return (key.Flags & RI_KEY_E0 ? mkb_KEY_CTRL_R : mkb_KEY_CTRL_L);
	case VK_SHIFT: return (key.Flags & RI_KEY_E0 ? mkb_KEY_SHIFT_R : mkb_KEY_SHIFT_L);
	case VK_MENU: return (key.Flags & RI_KEY_E0 ? mkb_KEY_ALT_R : mkb_KEY_ALT_L);
	case VK_LWIN: return mkb_KEY_WIN_L;
	case VK_RWIN: return mkb_KEY_WIN_R;

	case VK_APPS: return mkb_KEY_MENU;

	case VK_SPACE: return mkb_KEY_SPACE;
	case VK_OEM_7: return mkb_KEY_APOSTR;
	case VK_OEM_COMMA: return mkb_KEY_COMMA;
	case VK_OEM_MINUS: return mkb_KEY_MINUS;
	case VK_OEM_PERIOD: return mkb_KEY_PERIOD;
	case VK_OEM_2: return mkb_KEY_FSLASH;

	case VK_OEM_1: return mkb_KEY_SEMICOLON;
	case VK_OEM_PLUS: return mkb_KEY_EQUAL;

	case VK_OEM_4: return mkb_KEY_LBRACK;
	case VK_OEM_5: return mkb_KEY_BSLASH;
	case VK_OEM_6: return mkb_KEY_RBRACK;

	case VK_OEM_3: return mkb_KEY_BACKTICK;

	case VK_DIVIDE: return mkb_KEY_NUMDIV;
	case VK_MULTIPLY: return mkb_KEY_NUMMUL;
	case VK_SUBTRACT: return mkb_KEY_NUMSUB;
	case VK_ADD: return mkb_KEY_NUMADD;
	case VK_DECIMAL: return mkb_KEY_NUMPERIOD;

	default:
		printf("Unknown: %d\n", key.VKey);
	}

	return 0; // Unknown
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

	// WHY IS THE DAMN ALT KEY A SYSTEM KEY BUT THE WINDOWS KEY ISNT
	// WHY DOES IT HAVE A DIFFERENT MESSAGE
	if (data.data.keyboard.Message == WM_KEYDOWN || data.data.keyboard.Message == WM_SYSKEYDOWN)
		_mkb_keyboards[device]->lastKey = (uint8_t)key;
	_mkb_keyboards[device]->keys[key].state = 
		(data.data.keyboard.Message == WM_KEYDOWN || data.data.keyboard.Message == WM_SYSKEYDOWN);
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
	if (_mkb_isInit)
		return false;

	_mkb_isInit = true;

	msgWindow = NULL;
	devHndl = NULL;

	_mkb_keyboardNum = 0;
	_mkb_keyboards = NULL;
	_mkb_latestDev = -1;

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
	if (!_mkb_isInit)
		return mkb_DEVICE_NONE;

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


	_mkb_lastEvent = mkb_DEVICE_NONE;

	if (newCount > count)
	{
		if (newMaxCount > maxCount)
			_mkb_lastEvent = mkb_DEVICE_CONNECT;
		else
			_mkb_lastEvent = mkb_DEVICE_RECONNECT;
	}
	else if (newCount < count)
		_mkb_lastEvent = mkb_DEVICE_DISCONNECT;


	return _mkb_lastEvent;
}

uint8_t mkb_getLastEvent()
{
	return _mkb_lastEvent;
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

uint64_t mkb_getNthDevice(uint64_t index)
{
	uint64_t count = 0;
	for (uint64_t i = 0; i < mkb_deviceCount(); i++)
		if (_mkb_keyboards[i]->connected)
			if (++count == index)
				return index;
	return -1;
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

uint8_t mkb_lastKey(uint64_t index)
{
	if (index < mkb_deviceCount())
		return _mkb_keyboards[index]->lastKey;
	return 0;
}

bool mkb_capslockState()
{
	return ((GetKeyState(VK_CAPITAL) & 0x0001));
}

bool mkb_numlockState()
{
	return ((GetKeyState(VK_NUMLOCK) & 0x0001));
}

bool mkb_scrolllockState()
{
	return ((GetKeyState(VK_SCROLL) & 0x0001));
}

void mkb_shutdown()
{
	if (!_mkb_isInit)
		return;

	_mkb_isInit = false;

	DestroyWindow(msgWindow);
	UnregisterClassA("multikeyboard", GetModuleHandleA(NULL));
	msgWindow = NULL;

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
	free(_mkb_keyboards);


	_mkb_keyboardNum = 0;
	_mkb_keyboards = NULL;
	_mkb_latestDev = -1;
}

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#if (0) // bad visual studio
}
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

enum
{
	mkb_KEY_NULL = '\0',

	mkb_KEY_CAPSLOCK,
	mkb_KEY_NUMLOCK,
	mkb_KEY_SCROLLLOCK,

	mkb_KEY_ESC,
	mkb_KEY_BACKSP = '\b',
	mkb_KEY_TAB = '\t',
	mkb_KEY_ENTER = '\n',
	mkb_KEY_DEL,
	mkb_KEY_END,
	mkb_KEY_HOME,
	mkb_KEY_INSERT,
	mkb_KEY_PRINTSCR,
	mkb_KEY_PAGEUP,
	mkb_KEY_PAGEDOWN,
	mkb_KEY_UP,
	mkb_KEY_DOWN,
	mkb_KEY_LEFT,
	mkb_KEY_RIGHT,

	mkb_KEY_CTRL_L,
	mkb_KEY_CTRL_R,
	mkb_KEY_SHIFT_L,
	mkb_KEY_SHIFT_R,
	mkb_KEY_ALT_L,
	mkb_KEY_ALT_R,
	mkb_KEY_WIN_L,
	mkb_KEY_WIN_R,

	mkb_KEY_SPACE = ' ',
	mkb_KEY_APOSTR = '\'',
	mkb_KEY_COMMA = ',',
	mkb_KEY_MINUS = '-',
	mkb_KEY_PERIOD = '.',
	mkb_KEY_FSLASH = '/',

	mkb_KEY_0 = '0',
	mkb_KEY_1 = '1',
	mkb_KEY_2 = '2',
	mkb_KEY_3 = '3',
	mkb_KEY_4 = '4',
	mkb_KEY_5 = '5',
	mkb_KEY_6 = '6',
	mkb_KEY_7 = '7',
	mkb_KEY_8 = '8',
	mkb_KEY_9 = '9',

	mkb_KEY_SEMICOLON = ';',
	mkb_KEY_EQUAL = '=',

	mkb_KEY_A = 'A',
	mkb_KEY_B = 'B',
	mkb_KEY_C = 'C',
	mkb_KEY_D = 'D',
	mkb_KEY_E = 'E',
	mkb_KEY_F = 'F',
	mkb_KEY_G = 'G',
	mkb_KEY_H = 'H',
	mkb_KEY_I = 'I',
	mkb_KEY_J = 'J',
	mkb_KEY_K = 'K',
	mkb_KEY_L = 'L',
	mkb_KEY_M = 'M',
	mkb_KEY_N = 'N',
	mkb_KEY_O = 'O',
	mkb_KEY_P = 'P',
	mkb_KEY_Q = 'Q',
	mkb_KEY_R = 'R',
	mkb_KEY_S = 'S',
	mkb_KEY_T = 'T',
	mkb_KEY_U = 'U',
	mkb_KEY_V = 'V',
	mkb_KEY_W = 'W',
	mkb_KEY_X = 'X',
	mkb_KEY_Y = 'Y',
	mkb_KEY_Z = 'Z',

	mkb_KEY_LBRACK = '[',
	mkb_KEY_BSLASH = '\\',
	mkb_KEY_RBRACK = ']',

	mkb_KEY_BACKTICK = '`',

	mkb_KEY_NUMDIV,
	mkb_KEY_NUMMUL,
	mkb_KEY_NUMSUB,
	mkb_KEY_NUMADD,
	mkb_KEY_NUMENTER,
	mkb_KEY_NUMPERIOD,

	mkb_KEY_NUM0,
	mkb_KEY_NUM1,
	mkb_KEY_NUM2,
	mkb_KEY_NUM3,
	mkb_KEY_NUM4,
	mkb_KEY_NUM5,
	mkb_KEY_NUM6,
	mkb_KEY_NUM7,
	mkb_KEY_NUM8,
	mkb_KEY_NUM9,

	mkb_KEY_FN1,
	mkb_KEY_FN2,
	mkb_KEY_FN3,
	mkb_KEY_FN4,
	mkb_KEY_FN5,
	mkb_KEY_FN6,
	mkb_KEY_FN7,
	mkb_KEY_FN8,
	mkb_KEY_FN9,
	mkb_KEY_FN10,
	mkb_KEY_FN11,
	mkb_KEY_FN12,

	mkb_KEY_COUNT, // Some spaces were skipped, but this is easier
};

enum
{
	mkb_DEVICE_NONE,
	mkb_DEVICE_CONNECT,
	mkb_DEVICE_RECONNECT,
	mkb_DEVICE_DISCONNECT,
};

static const char* mkb_keyNames[mkb_KEY_COUNT] =
{
	"",
	"Capslock",
	"Numlock",
	"Scrolllock",

	"Escape",
	"", "", "",
	"Backspace",
	"Tab",
	"Enter",
	"Delete",
	"End",
	"Home",
	"Insert",
	"PrintScreen",
	"PageUp",
	"PageDown",
	"Up",
	"Down",
	"Left",
	"Right",

	"LeftControl",
	"RightControl",
	"LeftShift",
	"RightShift",
	"LeftAlt",
	"RightAlt",
	"LeftWin",
	"RightWin",

	"", "",

	"Space",
	"","","","","","",
	"Apostrophe",
	"","","","",
	"Comma",
	"Minus",
	"Period",
	"Forward Slash",

	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",

	"Semicolon",
	"",
	"Equal",
	"","","",

	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",

	"Left Bracket",
	"Back Slash",
	"Right Bracket",
	"","",

	"Backtick",

	"NumDivide",
	"NumMultiply",
	"NumSubtract",
	"NumAdd",
	"NumEnter",
	"NumPeriod",

	"Num0",
	"Num1",
	"Num2",
	"Num3",
	"Num4",
	"Num5",
	"Num6",
	"Num7",
	"Num8",
	"Num9",

	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12"
};

typedef struct mkb_Key
{
	uint8_t state : 1;
	uint8_t lastState : 1;
} mkb_Key;

typedef struct mkb_Keyboard
{
	mkb_Key keys[mkb_KEY_COUNT];
	bool connected;
	uint8_t lastKey;
	uint64_t keyCount;
	char* name;
} mkb_Keyboard;

extern uint64_t _mkb_keyboardNum;
extern mkb_Keyboard** _mkb_keyboards;
extern uint64_t _mkb_latestDev;

extern bool _mkb_isInit;
extern uint8_t _mkb_lastEvent;

// Initializes library (Will not work without this)
bool mkb_init(); 

// Updates the keyboard states, then returns if a device was connected or disconnected
uint8_t mkb_update(); 
// Returns if a device was connected or disconnected
uint8_t mkb_getLastEvent(); 

// Returns total number of devices that have connected, but may not be connected
// Should be used for iterating through all devices
uint64_t mkb_deviceCount(); 

// Should NOT be used for iterating through all devices
// Returns number of currently connected devices
uint64_t mkb_deviceConnectedCount(); 
// Returns the index of the last device to connect or disconnect
uint64_t mkb_getLatestDevice(); 

// Returns the index of the next device
// Used if you need to reorder things using the keyboards and need to refind the device indexes again
// Returns -1u64 if not found
// Ex: call with 0 to get the index of the first device, returns 2
// Ex: call with 5 to get the index of the fifth device, returns 8
uint64_t mkb_getNthDevice(uint64_t index);

// Returns the system device name
const char* mkb_deviceName(uint64_t index); 

// Returns if the key is pressed
bool mkb_keyState(uint64_t index, uint8_t key); 
// Returns if the key was pressed last update
bool mkb_keyLast(uint64_t index, uint8_t key); 
// Returns if the key is pressed this update, but wasn't last update
bool mkb_keyDown(uint64_t index, uint8_t key); 
// Returns if the key isn't pressed this update, but was last update
bool mkb_keyUp(uint64_t index, uint8_t key); 

// Returns the last key changed for a keyboard
uint8_t mkb_lastKey(uint64_t index); 

// These are global states, so they have to be different

// Returns the system capslock state
bool mkb_capslockState(); 
// Returns the system numberlock state
bool mkb_numlockState(); 
// Returns the system scrolllock state
bool mkb_scrolllockState();

// Frees memory and handles this library used
void mkb_shutdown(); 


#ifdef __cplusplus
}

class MultiKB
{
public:
	MultiKB() { mkb_init(); }

	uint8_t update() { return mkb_update(); }
	uint8_t getLastEvent() { return mkb_getLastEvent(); }

	uint64_t deviceCount() { return mkb_deviceCount(); }
	uint64_t deviceConnectedCount() { return mkb_deviceConnectedCount(); }
	uint64_t getLatestDevice() { return mkb_getLatestDevice(); }

	uint64_t getNthDevice(uint64_t index) { return mkb_getNthDevice(index); }

	const char* deviceName(uint64_t index) { return mkb_deviceName(index); }

	bool keyState(uint64_t index, uint8_t key) { return mkb_keyState(index, key); }
	bool keyLast(uint64_t index, uint8_t key) { return mkb_keyLast(index, key); }
	bool keyDown(uint64_t index, uint8_t key) { return mkb_keyDown(index, key); }
	bool keyUp(uint64_t index, uint8_t key) { return mkb_keyUp(index, key); }

	bool capslockState() { return mkb_capslockState(); }
	bool numlockState() { return mkb_numlockState(); }
	bool scrolllockState() { return mkb_scrolllockState(); }

	uint8_t lastKey(uint64_t index) { return mkb_lastKey(index); }

	~MultiKB() { mkb_shutdown(); }
};
#endif
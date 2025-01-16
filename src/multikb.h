#pragma once
#ifdef cplusplus
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

	mkb_KEY_CTRL,
	mkb_KEY_SHIFT,
	mkb_KEY_ALT,
	mkb_KEY_SYS,
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
	mkb_KEY_BSLASh = '\\',
	mkb_KEY_RBRACK = ']',

	mkb_KEY_BACKTICK = '`',

	mkb_KEY_NUMDIV,
	mkb_KEY_NUMMUL,
	mkb_KEY_NUMSUB,
	mkb_KEY_NUMADD,
	mkb_KEY_NUMENTER,
	mkb_KEY_NUMPERIOD,

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

	"Control",
	"Shift",
	"Alt",
	"System",
	"Escape",
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
	uint32_t keyCount;
	char* name;
} mkb_Keyboard;

extern uint64_t _mkb_keyboardNum;
extern mkb_Keyboard** _mkb_keyboards;
extern uint64_t _mkb_latestDev;

extern bool _mkb_isInit;


bool mkb_init();

uint8_t mkb_update();

uint64_t mkb_deviceCount();
uint64_t mkb_deviceConnectedCount();
uint64_t mkb_getLatestDevice();

const char* mkb_deviceName(uint64_t index);

bool mkb_keyState(uint64_t index, uint8_t key);
bool mkb_keyLast(uint64_t index, uint8_t key);
bool mkb_keyDown(uint64_t index, uint8_t key);
bool mkb_keyUp(uint64_t index, uint8_t key);

// These are global states, so they have to be different
bool mkb_capslockState();
bool mkb_numlockState();

void mkb_shutdown();


#ifdef cplusplus
}

class MultiKB
{
public:
	MultiKB() { mkb_init(); }

	uint8_t update() { mkb_update(); }

	uint64_t deviceCount() { mkb_deviceCount(); }
	uint64_t deviceConnectedCount() { mkb_deviceConnectedCount(); }
	uint64_t getLatestDevice() { mkb_getLatestDevice(); }

	const char* deviceName(uint64_t index) { mkb_deviceName(uint64_t index); }

	bool keyState(uint64_t index, uint8_t key) { mkb_keyState(uint64_t index, uint8_t key); }
	bool keyLast(uint64_t index, uint8_t key) { mkb_keyLast(uint64_t index, uint8_t key); }
	bool keyDown(uint64_t index, uint8_t key) { mkb_keyDown(uint64_t index, uint8_t key); }
	bool keyUp(uint64_t index, uint8_t key) { mkb_keyUp(uint64_t index, uint8_t key); }

	~MultiKB() { mkb_shutdown(); }
};
#endif
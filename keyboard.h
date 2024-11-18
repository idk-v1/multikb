#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

	enum
	{
		key_Null = '\0',
		key_Capslock,
		key_Numlock,

		key_Control,
		key_Shift,
		key_Alt,
		key_System,
		key_Escape,
		key_BackSp = '\b',
		key_Tab = '\t',
		key_Enter = '\n',
		key_Delete,
		key_End,
		key_Home,
		key_Insert,
		key_PrintScr,
		key_PageUp,
		key_PageDown,
		key_Up,
		key_Down,
		key_Left,
		key_Right,

		key_Num0,
		key_Num1,
		key_Num2,
		key_Num3,
		key_Num4,
		key_Num5,
		key_Num6,
		key_Num7,
		key_Num8,
		key_Num9,

		key_Space = ' ',
		key_Apostr = '\'',
		key_Comma = ',',
		key_Minus = '-',
		key_Period = '.',
		key_FSlash = '/',

		key_0 = '0',
		key_1 = '1',
		key_2 = '2',
		key_3 = '3',
		key_4 = '4',
		key_5 = '5',
		key_6 = '6',
		key_7 = '7',
		key_8 = '8',
		key_9 = '9',

		key_Semicolon = ';',
		key_Equal = '=',

		key_A = 'A',
		key_B = 'B',
		key_C = 'C',
		key_D = 'D',
		key_E = 'E',
		key_F = 'F',
		key_G = 'G',
		key_H = 'H',
		key_I = 'I',
		key_J = 'J',
		key_K = 'K',
		key_L = 'L',
		key_M = 'M',
		key_N = 'N',
		key_O = 'O',
		key_P = 'P',
		key_Q = 'Q',
		key_R = 'R',
		key_S = 'S',
		key_T = 'T',
		key_U = 'U',
		key_V = 'V',
		key_W = 'W',
		key_X = 'X',
		key_Y = 'Y',
		key_Z = 'Z',

		key_LBracket = '[',
		key_BSlash = '\\',
		key_RBracket = ']',

		key_Grave = '`',

		key_NumDiv,
		key_NumMult,
		key_NumSub,
		key_NumAdd,
		key_NumEnter,
		key_NumPeriod,

		key_Fn1,
		key_Fn2,
		key_Fn3,
		key_Fn4,
		key_Fn5,
		key_Fn6,
		key_Fn7,
		key_Fn8,
		key_Fn9,
		key_Fn10,
		key_Fn11,
		key_Fn12,

		key_Count, // Some spaces were skipped, but this is easier
	};

	static const char* keyNames[key_Count] =
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

		"Grave",

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

	typedef struct
	{
		char keys[key_Count];
		bool state;
		char* devName;
	} Keyboard;

	typedef struct _OSKBInfo _OSKBInfo;

	typedef struct
	{
		// Keyboard take up a decent amount of memory, 
		// and have no reason to be allocated contiguously.
		// I know that modern PCs have enough memory to hold 
		// 128 * (number of keyboards) bytes contiguously,
		// but this was my reasoning for an array of pointers 
		// instead of an array of keyboards.
		Keyboard** kb;
		uint32_t numKB;
		_OSKBInfo* _osInfo;
		bool useToggle; // On by default | Set after multiKB_Setup() call
	} KBManager;

	/*
		Keyboard Manager
			Array of Pointers to a Keyboard
				Keys
					bit 0 - raw state
					bit 1 - current state
					bit 2 - last state
				Is Connected
				Device Name used for comparing new device 
				to existing disconnected device
			Number of Keyboards
			OS Specific info, like handles
			Treats keys like capslock or numlock as toggles 
			instead of pressed or released
	*/

	bool multiKB_Setup(KBManager* kbMgr);

	void multiKB_Update(KBManager* kbMgr);

	void multiKB_Shutdown(KBManager* kbMgr);

	inline bool multiKB_Key(KBManager* kbMgr, uint32_t index, uint32_t key)
	{
		if (kbMgr)
			if (index < kbMgr->numKB)
				if (key < key_Count)
					return (kbMgr->kb[index]->keys[key] & 2) >> 1;
		return false;
	}

	inline bool multiKB_KeyLast(KBManager* kbMgr, uint32_t index, uint32_t key)
	{
		if (kbMgr)
			if (index < kbMgr->numKB)
				if (key < key_Count)
					return (kbMgr->kb[index]->keys[key] & 4) >> 2;
		return false;
	}

	inline bool multiKB_KeyPress(KBManager* kbMgr, uint32_t index, uint32_t key)
	{
		return !multiKB_KeyLast(kbMgr, index, key) && multiKB_Key(kbMgr, index, key);
	}

	inline bool multiKB_KeyRelease(KBManager* kbMgr, uint32_t index, uint32_t key)
	{
		return multiKB_KeyLast(kbMgr, index, key) && !multiKB_Key(kbMgr, index, key);
	}

#ifdef __cplusplus
}
#endif
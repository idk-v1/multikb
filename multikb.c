#include "multikb.h"

bool multiKB_IsConnected(KBManager* kbMgr, uint32_t index)
{
	if (kbMgr)
		if (index < kbMgr->numKB)
			return kbMgr->kb[index]->state;
}

bool multiKB_Key(KBManager* kbMgr, uint32_t index, uint32_t key)
{
	if (kbMgr)
		if (index < kbMgr->numKB)
			if (key < key_Count)
				return (kbMgr->kb[index]->keys[key] & 2) >> 1;
	return false;
}

bool multiKB_KeyLast(KBManager* kbMgr, uint32_t index, uint32_t key)
{
	if (kbMgr)
		if (index < kbMgr->numKB)
			if (key < key_Count)
				return (kbMgr->kb[index]->keys[key] & 4) >> 2;
	return false;
}

bool multiKB_KeyPress(KBManager* kbMgr, uint32_t index, uint32_t key)
{
	return !multiKB_KeyLast(kbMgr, index, key) && multiKB_Key(kbMgr, index, key);
}

bool multiKB_KeyRelease(KBManager* kbMgr, uint32_t index, uint32_t key)
{
	return multiKB_KeyLast(kbMgr, index, key) && !multiKB_Key(kbMgr, index, key);
}

const char* multiKB_GetDeviceName(KBManager* kbMgr, uint32_t index)
{
	if (!kbMgr)
		return NULL;

	if (index < kbMgr->numKB)
		return kbMgr->kb[index]->devName;
}

uint32_t multiKB_GetDeviceKeyCount(KBManager* kbMgr, uint32_t index)
{
	if (!kbMgr)
		return 0;

	if (index < kbMgr->numKB)
		return kbMgr->kb[index]->keyCount;
}

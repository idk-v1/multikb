#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

	extern int allocCount;

	inline void* mallocCount(size_t _Size, const char* file, int ln)
	{
		void* ptr = malloc(_Size);
		allocCount++;
		printf("+%3i %p @%s:%d\n", allocCount, ptr, file, ln);
		return ptr;
	}

	inline void* reallocCount(void* _Block, size_t _Size, const char* file, int ln)
	{
		void* ptr = realloc(_Block, _Size);
		if (_Block == NULL)
		{
			allocCount++;
			printf("+%3i %p @%s:%d\n", allocCount, ptr, file, ln);
		}
		else
		{
			printf("-%3i %p @%s:%d\n", allocCount - 1, _Block, file, ln);
			printf("+%3i %p @%s:%d\n", allocCount, ptr, file, ln);
		}
		return ptr;
	}

	inline void freeCount(void* _Block, const char* file, int ln)
	{
		free(_Block);
		allocCount--;
		printf("-%3i %p @%s:%d\n", allocCount, _Block, file, ln);
	}

#define malloc(_Size) mallocCount(_Size, __FILENAME__, __LINE__)
#define realloc(_Block, _Size) reallocCount(_Block, _Size, __FILENAME__, __LINE__)
#define free(_Block) freeCount(_Block, __FILENAME__, __LINE__)

#ifdef __cplusplus
}
#endif
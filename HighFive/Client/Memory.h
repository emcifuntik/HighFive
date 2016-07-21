#pragma once
#include <sstream>
#include <string>
#include <Windows.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

class Memory
{
	void *address;
	static bool memoryCompare(const BYTE *data, const BYTE *pattern, size_t length);
	static bool memoryCompare(const BYTE *data, const BYTE *pattern, const char *mask);
public:
	static ptrdiff_t baseDiff;
	template<typename T, T Value>
	static T Return()
	{
		return Value;
	}

	Memory(UINT64 address);
	~Memory();

	void put(const char *value);
	template <typename T> void put(T value);
	template <typename T> void put(void *address, T value);
	template <typename T> void jump(T func);
	template <typename T> void call(T func);
	
	Memory operator+(int offset)
	{
		Memory *m = new Memory(*this);
		m->address = (void*)((intptr_t)m->address + offset);
		return (*m);
	}

	void nop(size_t length);

	static Memory *Find(const char* pattern);
	static Memory *Find_t(const char* text);
	static void Init();
};

template<typename T>
inline void Memory::put(T value)
{
	unsigned long dwProtectOld;
	VirtualProtect((LPVOID)address, 2, PAGE_EXECUTE_READWRITE, &dwProtectOld);
	memcpy(address, &value, sizeof(T));
	VirtualProtect((LPVOID)address, 2, dwProtectOld, NULL);
}

template<typename T>
inline void Memory::put(void * address, T value)
{
	unsigned long dwProtectOld;
	VirtualProtect((LPVOID)address, 2, PAGE_EXECUTE_READWRITE, &dwProtectOld);
	memcpy(address, &value, sizeof(T));
	VirtualProtect((LPVOID)address, 2, dwProtectOld, NULL);
}

template<typename T>
inline void Memory::jump(T func)
{
	put(address, (uint8_t)0xE9);
	put((void*)((size_t*)address + 1), (intptr_t)func - (intptr_t)baseDiff - 5);
}

template<typename T>
inline void Memory::call(T func)
{
	put(address, (uint8_t)0xE8);
	put((void*)((size_t*)address + 1), (intptr_t)func - (intptr_t)baseDiff - 5);
}

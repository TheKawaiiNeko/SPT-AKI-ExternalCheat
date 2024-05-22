#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <string_view>
#include <thread>
#include <vector>

class Memory
{
private:
	char TargetName[32] = "EscapeFromTarkov";
	HWND TargetHwnd;
public:
	DWORD PID;
	uint64_t BaseAddress;
	HANDLE pHandle;

	bool Init();
	uintptr_t GetModuleBase(const std::string moduleName);

	template <typename T>
	constexpr const T Read(const uintptr_t& address) const noexcept
	{
		T value = { };
		ReadProcessMemory(pHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}
	template <typename T>
	constexpr void Write(const uintptr_t& address, const T& value) const noexcept
	{
		WriteProcessMemory(pHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
	std::string ReadString(const uint64_t address, const int size)
	{
	char read_val[4096]{};
	ReadProcessMemory(pHandle, (void*)(address), read_val, sizeof(size), nullptr);

	return read_val;
	}
	uintptr_t ReadChain(uintptr_t address, const std::vector<uint64_t>& offsets)
	{
		uintptr_t result = Read<uint64_t>(address + offsets.at(0));
		for (int i = 1; i < offsets.size(); i++)
			result = Read<uintptr_t>(result + offsets.at(i));

		return result;
	}
};

extern Memory m;

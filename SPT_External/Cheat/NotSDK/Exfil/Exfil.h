#pragma once
#include "../NotSDK.h"

/*
	[ Status ]
	1 : Close
	2 : NotReady
	4 : Open
	6 : Ready (Labのエレベーターとかがこれだったはず
*/

class Exfil
{
private:
	bool IsValid();
	Vector3 GetTransformPosition(uintptr_t transform);
public:
	uintptr_t ptr;

	int status;
	Vector3 pos;
	char Name[256]{};

	bool Update()
	{
		if (!IsValid())
			return false;

		status = m.Read<int>(ptr + 0xA8);

		if (status == 1)
			return false;

		uintptr_t TransformInternal = m.ReadChain(ptr, { 0x10, 0x30, 0x30, 0x8, 0x28, 0x10 });
		pos = GetTransformPosition(TransformInternal);

		if (pos == Vector3(0.f, 0.f, 0.f))
			return false;

		uintptr_t eSetting = m.Read<uintptr_t>(ptr + 0x58);
		uintptr_t NamePtr = m.Read<uintptr_t>(eSetting + 0x10);

		if (!NamePtr)
			return false;

		int length = m.Read<int>(NamePtr + 0x10);

		for (int i = 0; i < length; i++)
			Name[i] = m.Read<char>(NamePtr + 0x14 + (i * 0x2));

		return true;
	}
};

#pragma once
#include "../NotSDK.h"

class Player
{
private:
	bool IsValid();
	Vector3 GetTransformPosition(uintptr_t transform);
public:
	uintptr_t ptr;
	uintptr_t profile;
	uintptr_t info;

	int Team;
	int Side;
	float Health;
	float MaxHealth;
	Vector3 BasePos;
	Vector3 HeadPos;
	Vector3 NeckPos;
	std::string Name;

	bool Update()
	{
		// 各ベースポインタを取得
		profile = m.Read<uintptr_t>(ptr + Offsets::Player::Profile);
		info = m.Read<uintptr_t>(profile + 0x28);

		// Side
		Side = m.Read<int>(info + 0x70); // 1 : Usec, 2 : Bear, 4 : Scavs

		// Health
		uintptr_t m_pHealthController = m.Read<uint64_t>(ptr + Offsets::Player::HealthController);
		uintptr_t m_pHealthBody = m.Read<uint64_t>(m_pHealthController + 0x68);
		uintptr_t m_pBodyController = m.Read<uint64_t>(m_pHealthBody + 0x18);

		for (int h = 0; h < 7; h++)
		{
			auto body_part = m.Read<uintptr_t>(m_pBodyController + 0x30 + (h * 0x18));
			auto health_container = m.Read<uintptr_t>(body_part + 0x10);
			Health += m.Read<float>(health_container + 0x10);
			MaxHealth += m.Read<float>(health_container + 0x14);
		}

		uintptr_t BoneMatrixPtr = m.ReadChain(ptr, { 0xA8, 0x28, 0x28, 0x10 });

		// LOL
		if (BoneMatrixPtr)
		{
			uintptr_t BasePtr = m.Read<uintptr_t>(BoneMatrixPtr + 0x20 + PlayerBones::HumanBase * 8);
			uintptr_t base_bone = m.Read<uintptr_t>(BasePtr + 0x10);
			BasePos = GetTransformPosition(base_bone);

			uintptr_t hPtr = m.Read<uintptr_t>(BoneMatrixPtr + 0x20 + PlayerBones::HumanHead * 8);
			uintptr_t hBone = m.Read<uintptr_t>(hPtr + 0x10);
			HeadPos = GetTransformPosition(hBone);

			uintptr_t nPtr = m.Read<uintptr_t>(BoneMatrixPtr + 0x20 + PlayerBones::HumanNeck * 8);
			uintptr_t nBone = m.Read<uintptr_t>(nPtr + 0x10);
			NeckPos = GetTransformPosition(nBone);
		}

		return true;
	}
};

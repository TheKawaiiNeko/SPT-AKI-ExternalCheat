#pragma once
#include "Struct.h"
#include "..\..\Utils\Memory\Memory.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

// Credit
// https://www.unknowncheats.me/forum/escape-from-tarkov/579954-dma-based-esp.html
// https://www.unknowncheats.me/forum/escape-from-tarkov/482418-2d-map-dma-radar-wip.html

// SPT-AKI 3.8.3 (EFT GameVer : 0.14.1.2.29197)
namespace Offsets
{
    constexpr auto GameObjectManager = 0x17FFD28;

    namespace LocalGameWorld 
    {
        constexpr auto RegisteredPlayers = 0xF0;
    }
    namespace Player
    {
        constexpr auto Profile = 0x588;
        constexpr auto HealthController = 0x5C8;
    }
    namespace Profile // EFT.Profile
    {
        constexpr auto Id = 0x10;
        constexpr auto AccountId = 0x18; 
        constexpr auto PlayerInfo = 0x28; 
        constexpr auto Stats = 0xF0; 
    }
    namespace PlayerInfo
    {
        constexpr auto Nickname = 0x10;
        constexpr auto MainProfileNickname = 0x18;
        constexpr auto GroupId = 0x20; // str 
		constexpr auto TeamId = 0x28; // str
        constexpr auto Settings = 0x50; 
        constexpr auto PlayerSide = 0xE8;
    }
}

class Tarkov
{
private:
    uintptr_t GetObjectFromList(uintptr_t listPtr, uintptr_t lastObjectPtr, const char* objectName);
public:
    // EFT Data
　　GameObjectManager GOM{};
    uintptr_t GameWorld = 0;
    uintptr_t localGameWorld = 0;
    uintptr_t fpsCamera = 0;
　　Matrix MatrixFPS{};

　　// SomeFunctions
　　bool InitAddress();
　　bool GetCamera();
　　void UpdateViewMatrix();
　　bool WorldToScreen(const Vector3& position, Vector2& vOut);
};

class CPlayer
{
private:
	std::vector<PlayerBones> BoneList = { HumanPelvis, HumanHead, HumanLForearm2, HumanLPalm, HumanRForearm2,HumanRPalm, HumanLThigh2, HumanLFoot, HumanRThigh2, HumanRFoot };
	Vector3 GetTransformPosition(uintptr_t transform)
	{
		__m128 result{};

		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		TransformAccessReadOnly pTransformAccessReadOnly = m.Read<TransformAccessReadOnly>(transform + 0x38);
		unsigned int Index = m.Read<unsigned int>(transform + 0x40);
		TransformData transform_data = m.Read<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

		if (transform_data.pTransformArray && transform_data.pTransformIndices)
		{
			result = m.Read<__m128>(transform_data.pTransformArray + 0x30 * Index);
			int transform_index = m.Read<int>(transform_data.pTransformIndices + 0x4 * Index);
			int safe = 0;
			while (transform_index >= 0 && safe++ < 200)
			{
				Matrix34 matrix = m.Read<Matrix34>(transform_data.pTransformArray + 0x30 * transform_index);

				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x00));	// xxxx
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x55));	// yyyy
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x8E));	// zwxy
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xDB));	// wzyw
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0xAA));	// zzzz
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix.vec1), 0x71));	// yxwy
				__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix.vec2), result);

				result = _mm_add_ps(_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), *(__m128*)(&matrix.vec0));

				transform_index = m.Read<int>(transform_data.pTransformIndices + 0x4 * transform_index);
			}
		}

		return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
	}
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

	bool IsValid() noexcept
	{
		return ptr;
	}
	bool IsDead() noexcept
	{
		return false; // ToDo
	}
	bool Update()
	{
		// GetBasePointer
		profile = m.Read<uint64_t>(ptr + Offsets::Player::Profile);
		info = m.Read<uint64_t>(profile + 0x28);

		// Side
		Side = m.Read<int>(info + 0x70); // 1 : Usec, 2 : Bear, 4 : Scavs

		// Health
		uint64_t m_pHealthController = m.Read<uint64_t>(ptr + Offsets::Player::HealthController);
		uint64_t m_pHealthBody = m.Read<uint64_t>(m_pHealthController + 0x68);
		uint64_t m_pBodyController = m.Read<uint64_t>(m_pHealthBody + 0x18);

		for (int h = 0; h < 7; h++)
		{
			auto body_part = m.Read<uintptr_t>(m_pBodyController + 0x30 + (h * 0x18));
			auto health_container = m.Read<uintptr_t>(body_part + 0x10);
			Health += m.Read<float>(health_container + 0x10);
			MaxHealth += m.Read<float>(health_container + 0x14);
		}

		if (IsDead())
			return false;

		uintptr_t BoneMatrixPtr = m.ReadChain(ptr, { 0xA8, 0x28, 0x28, 0x10 });

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
	uintptr_t GetBoneMatrixPtr()
	{
		return m.ReadChain(ptr, { 0xA8, 0x28, 0x28, 0x10 });
	}
};

extern float GetDistance(Vector3 value1, Vector3 value2);

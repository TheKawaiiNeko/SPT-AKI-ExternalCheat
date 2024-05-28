#pragma once
#include "Struct.h"
#include "..\..\Utils\Memory\Memory.h"

// Credit
// https://www.unknowncheats.me/forum/escape-from-tarkov/579954-dma-based-esp.html
// https://www.unknowncheats.me/forum/escape-from-tarkov/482418-2d-map-dma-radar-wip.html

// SPT-AKI 3.8.3 (EFT GameVer : 0.14.1.2.29197)
namespace Offsets
{
    constexpr auto GameObjectManager = 0x17FFD28;

    namespace LocalGameWorld  // EFT.GameWorld
    {
		constexpr auto ExfilController = 0x18;
        constexpr auto LootList = 0xC8;
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

extern float GetDistance(Vector3 value1, Vector3 value2);

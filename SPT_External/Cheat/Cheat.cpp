#include "Cheat.h"

void Cheat::UpdateList()
{
	while (g.Run)
	{
		std::vector<CPlayer> TempList;
        Sleep(5);

        if (!EFT.InitAddress())
            continue;

        uintptr_t RegisteredPlayer = m.Read<uintptr_t>(EFT.localGameWorld + Offsets::LocalGameWorld::RegisteredPlayers);
        UnityList PlayerArray = m.Read<UnityList>(RegisteredPlayer + 0x10);

        if (!PlayerArray.Base || PlayerArray.Count <= 0)
            continue;

        local.ptr = m.Read<uintptr_t>(PlayerArray.Base + 0x20);

        for (int i = 1; i < PlayerArray.Count; i++)
        {
            CPlayer player{};
            player.ptr = m.Read<uintptr_t>(PlayerArray.Base + 0x20 + (i * 8));

            if (!player.IsValid())
                continue;

            TempList.push_back(player);
        }

        EntityList = TempList;
        TempList.clear();

        Sleep(1000);
	}
}

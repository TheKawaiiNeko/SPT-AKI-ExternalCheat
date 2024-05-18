#include "Cheat.h"

void Cheat::UpdateList()
{
	while (g.Run)
	{
		std::vector<uintptr_t> TempList;

        uintptr_t OnlineUsers = m.Read<uintptr_t>(EFT.localGameWorld + Offsets::LocalGameWorld::RegisteredPlayers);

        if (!OnlineUsers)
        {
            Sleep(5);
            continue;
        }

        int pCount = m.Read<int>(OnlineUsers + offsetof(List, itemCount));
        uintptr_t pListPtr = m.Read<uintptr_t>(OnlineUsers + offsetof(List, listBase));

        if (!pListPtr || pCount <= 0)
        {
            Sleep(5);
            continue;
        }

        for (int i = 0; i < pCount; i++)
        {
            // Update PlayerData
            uintptr_t player = m.Read<uintptr_t>(pListPtr + (offsetof(ListInternal, firstEntry) + (i * 8)));

            if (!player)
                continue;

            TempList.push_back(player);
        }

        EntityList = TempList;
        TempList.clear();

        Sleep(250);
	}
}
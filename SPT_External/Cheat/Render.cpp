#include "Cheat.h"

void Cheat::RenderInfo()
{
    // FrameRate
    ImGui::GetBackgroundDrawList()->AddText(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), std::to_string((int)ImGui::GetIO().Framerate).c_str());

    // Crosshair?
    // FOV Circle?
}

void Cheat::RenderMenu()
{
    ImGui::SetNextWindowSize(ImVec2(300.f, 250.f));
    ImGui::Begin("SPT-AKI [ EXTERNAL ]", &g.ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Visual");
    ImGui::Separator();
    ImGui::Checkbox("ESP", &g.ESP);
    ImGui::Checkbox("Exfil ESP", &g.ExfilESP);

    ImGui::NewLine();

    ImGui::Text("ESP Options");
    ImGui::Separator();
    ImGui::Checkbox("ESP Box", &g.ESP_Box);
    ImGui::Checkbox("ESP Line", &g.ESP_Line);
    ImGui::Checkbox("ESP Healthbar", &g.ESP_Healthbar);
    ImGui::Checkbox("ESP Distance", &g.ESP_Distance);

    ImGui::End();
}

void Cheat::RenderESP()
{
    // GetLocal
    Player* pLocal = &local;
    if (!pLocal->Update())
        return;

    EFT.UpdateViewMatrix();

    // Player
    for (auto& player : EntityList)
    {
        Player* pEntity = &player;
        if (!pEntity->Update())
            continue;

        float distance = GetDistance(pLocal->BasePos, pEntity->BasePos);

        Vector2 g_Screen{}, g_HeadScreen{}, g_NeckScreen{};
        if (!EFT.WorldToScreen(pEntity->BasePos, g_Screen) || !EFT.WorldToScreen(pEntity->HeadPos, g_HeadScreen) || !EFT.WorldToScreen(pEntity->NeckPos, g_NeckScreen))
            continue;

        float NeckToHead = (g_NeckScreen.y - g_HeadScreen.y) * 3.f;
        float pHeight = g_Screen.y - g_HeadScreen.y + NeckToHead;
        float pWidth = pHeight / 3.5f; // Half

        // Line
        if (g.ESP_Line)
            DrawLine(ImVec2(g.GameSize.right / 2.f, g.GameSize.bottom), ImVec2(g_Screen.x, g_Screen.y), ImColor(1.f, 0.f, 0.f, 0.f), 1.f);

        // 2D Box
        if (g.ESP_Box)
        {
            DrawLine(ImVec2(g_Screen.x - pWidth, g_HeadScreen.y - NeckToHead), ImVec2(g_Screen.x + pWidth, g_HeadScreen.y - NeckToHead), ImColor(1.f, 0.f, 0.f, 1.f), 1.f);
            DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y), ImVec2(g_Screen.x - pWidth, g_HeadScreen.y - NeckToHead), ImColor(1.f, 0.f, 0.f, 1.f), 1.f);
            DrawLine(ImVec2(g_Screen.x + pWidth, g_Screen.y), ImVec2(g_Screen.x + pWidth, g_HeadScreen.y - NeckToHead), ImColor(1.f, 0.f, 0.f, 1.f), 1.f);
            DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y), ImVec2(g_Screen.x + pWidth, g_Screen.y), ImColor(1.f, 0.f, 0.f, 1.f), 1.f);
        }

        // Healthbar
        if (g.ESP_Healthbar)
            HealthBar(g_Screen.x - pWidth - 4.f, g_Screen.y, 2.f, -pHeight, pEntity->Health, pEntity->MaxHealth);

        // Distance
        if (g.ESP_Distance)
        {
            std::string DistStr = std::to_string((int)distance) + "m";
            String(ImVec2(g_Screen.x - (ImGui::CalcTextSize(DistStr.c_str()).x / 2.f), g_Screen.y), ImColor(1.f, 1.f, 1.f, 1.f), DistStr.c_str());
        }
    }
    
    // Exfil
    if (g.ExfilESP)
    {
        for (const auto& exfil : ExfilList)
        {
            Vector2 ExfilScreen{};
            if (!EFT.WorldToScreen(exfil.pos, ExfilScreen))
                continue;

            String(ImVec2(ExfilScreen.x - ImGui::CalcTextSize(exfil.Name).x, ExfilScreen.y), ImColor(1.f, 1.f, 1.f, 1.f), exfil.Name);
        }
    }
}
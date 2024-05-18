#include "Cheat.h"

void Cheat::RenderInfo()
{
    // FrameRate
    ImGui::GetBackgroundDrawList()->AddText(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), std::to_string((int)ImGui::GetIO().Framerate).c_str());

    if (EFT.InitAddress())
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(8.f, 24.f), ImColor(1.f, 1.f, 1.f, 1.f), std::to_string((int)EFT.localGameWorld).c_str());

    // Crosshair?
    // FOV Circle?
}

void Cheat::RenderMenu()
{
    ImGui::SetNextWindowSize(ImVec2(600.f, 550.f));
    ImGui::Begin("SPT-AKI [ EXTERNAL ]", &g.ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Checkbox("ESP", &g.ESP);
    ImGui::Checkbox("ESP Line", &g.ESP_Line);
    ImGui::Checkbox("ESP Distance", &g.ESP_Distance);

    ImGui::End();
}

void Cheat::RenderESP()
{
    // GetLocal


    for (auto player : EntityList)
    {
        CPlayer pl, *pEntity = &pl;
        pEntity->ptr = player;
        if (!pEntity->Update())
            continue;

        Vector2 g_Screen = EFT.WorldToScreen(pEntity->BasePos);
        Vector2 g_headScreen = EFT.WorldToScreen(pEntity->HeadPos);
        Vector2 g_Neck = EFT.WorldToScreen(pEntity->NeckPos);

        if (g_Screen == Vector2(0.f, 0.f) || g_headScreen == Vector2(0.f, 0.f) || g_Neck == Vector2(0.f, 0.f))
            continue;

        float NeckToHead = (g_Neck.y - g_headScreen.y) * 3.f;
        float pHeight = g_Screen.y - g_headScreen.y + NeckToHead;
        float pWidth = pHeight / 3.5f;

        //float Distance = GetDistance(pEntity->BasePos, );

        // 2D Box
        DrawLine(ImVec2(g_Screen.x - pWidth, g_headScreen.y - NeckToHead), ImVec2(g_Screen.x + pWidth, g_headScreen.y - NeckToHead), ImColor(1.f, 0.f, 0.f, 1.f), 1.f);
        DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y), ImVec2(g_Screen.x - pWidth, g_headScreen.y - NeckToHead), ImColor(1.f, 0.f, 0.f, 1.f), 1.f);
        DrawLine(ImVec2(g_Screen.x + pWidth, g_Screen.y), ImVec2(g_Screen.x + pWidth, g_headScreen.y - NeckToHead), ImColor(1.f, 0.f, 0.f, 1.f), 1.f);
        DrawLine(ImVec2(g_Screen.x - pWidth, g_Screen.y), ImVec2(g_Screen.x + pWidth, g_Screen.y), ImColor(1.f, 0.f, 0.f, 1.f), 1.f);
    
        // Healthbar
        HealthBar(g_Screen.x - pWidth - 4.f, g_Screen.y, 2.f, -pHeight, pEntity->Health, pEntity->MaxHealth);
    }
}
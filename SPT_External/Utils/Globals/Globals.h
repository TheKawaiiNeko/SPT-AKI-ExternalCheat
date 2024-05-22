#pragma once
#include <Windows.h>

struct Globals
{
	// System
	bool Run = false;
	int MenuKey = VK_INSERT;

	// Status
	bool ShowMenu = false;
	RECT GameSize{};
	POINT GamePoint{};

	// Visual
	bool ESP = true;
	bool ESP_Box = true;
	bool ESP_Line = true;
	bool ESP_Healthbar = true;
	bool ESP_Distance = false;
};

extern struct Globals g;

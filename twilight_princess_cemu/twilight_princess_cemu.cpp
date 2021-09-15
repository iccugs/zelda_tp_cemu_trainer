#include "stdafx.h"

#include <iostream>
#include <vector>
#include <Windows.h>

#include "proc.h"
#include "mem.h"
#include "ascii.h"

bool bHealth = false;

void Menu()
{
	std::cout << "HOTKEY = FUNCTION = STATUS\n";
	std::cout << "==========================\n";
	std::cout << "NUM0 = Infinite Health = " << bHealth << "\n\n";
	std::cout << "INSERT = Exit Trainer\n";
}

int main()
{
	HANDLE hProcess = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 9);
	uintptr_t moduleBase = 0;

	DWORD procId = GetProcId(L"Cemu.exe");

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		moduleBase = GetModuleBaseAddress(procId, L"Cemu.exe");
		AsciiArt();
		getchar();
		ClearScreen();
		Menu();
	}

	else
	{
		std::cout << "Cemu process not found. Press enter to exit...\n";
		getchar();
		return(0);
	}

	DWORD dwExit = 0;

	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			bHealth = !bHealth;

			if (bHealth)
			{
				mem::NopEx((BYTE*)(moduleBase + 0x2986B89194E), 6, hProcess);

				ClearScreen();
				Menu();
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBase + 0x2986B89194E), (BYTE*)"\x66\x45\x89\x74\x3D\x04", 6, hProcess);

				ClearScreen();
				Menu();
			}
		}

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			return(0);
		}

		Sleep(100);

	}

	ClearScreen();
	std::cout << "Cemu process not found. Press enter to exit...\n";
	getchar();
	return(0);

}
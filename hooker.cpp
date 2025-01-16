#include <Windows.h>
#include <iostream>
#include <fstream>
#include <detours.h>  

typedef void (WINAPI *OutputDebugStringW_t)(LPCWSTR lpOutputString);
OutputDebugStringW_t originalOutputDebugString = NULL;
void WINAPI myOutputDebugString(LPCWSTR lpOutputString)
{
    std::wofstream logFile("roblox_console_log.txt", std::ios_base::app);
    logFile << lpOutputString << std::endl;
    logFile.close();

    originalOutputDebugString(lpOutputString);
}
void Hook()
{
    originalOutputDebugString = (OutputDebugStringW_t)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "OutputDebugStringW");
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)originalOutputDebugString, myOutputDebugString);
    DetourTransactionCommit();
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        Hook();  // print console out put When the dll is hooked ofc ??
    }
    return TRUE;
}
// its a Testing dll i dont really think it will work???
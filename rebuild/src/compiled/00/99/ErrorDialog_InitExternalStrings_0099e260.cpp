// bbblibrary/errordialog.cpp, InitExternalStrings @ 0x0099E260 (Ego_r
// @InitExternalStrings@0, an extern "C" fastcall function; locals hKey,
// Buffer[260], dwDataSize, wLanguageId, dwType; built with /GS). Loads
// strings.dll (fatal if missing), takes the UI language from
// HKLM\...\Fable\1.0 LangID (a primary language id gets SUBLANG_DEFAULT),
// then fills the crash-handler texts from the DLL or built-in English.
#include <string.h>

struct HINSTANCE__;
struct HWND__;

extern "C" __declspec(dllimport) HINSTANCE__* __stdcall LoadLibraryA(const char* name);
extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentDirectoryA(unsigned long size, char* buffer);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(HWND__* window, const char* text, const char* caption,
                                                          unsigned int type);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int code);
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount();
extern "C" __declspec(dllimport) long __stdcall RegOpenKeyExA(void* key, const char* subKey,
    unsigned long options, unsigned long access, void** result);
extern "C" __declspec(dllimport) long __stdcall RegQueryValueExA(void* key, const char* valueName,
    unsigned long* reserved, unsigned long* type, unsigned char* data, unsigned long* dataSize);
extern "C" __declspec(dllimport) long __stdcall RegCloseKey(void* key);

extern "C" int __fastcall RetrieveString(HINSTANCE__* hModule, unsigned long dwStringID, char* pBuffer,
                                         unsigned long dwBufferSize);

// Ego_r errordialog.cpp globals (retail 0x0138E060, 0x013BCE38, 0x013BD0C8..0x013BD6EC).
extern unsigned short gApplicationLanguage;
extern char Text_GatheringExceptionData[256];
extern char Text_Exception[256];
extern "C" char Text_EULA[32];
extern "C" char Text_Switches[1024];
extern "C" HINSTANCE__* hStrings;
extern "C" unsigned long gamestarttime;

extern "C" void __fastcall InitExternalStrings()
{
    void* hKey;
    char Buffer[260];
    unsigned long dwDataSize;
    unsigned long wLanguageId;
    unsigned long dwType;

    hStrings = LoadLibraryA("strings.dll");
    if (hStrings == 0)
    {
        GetCurrentDirectoryA(260, Buffer);
        strcat(Buffer, "\\strings.dll is missing.");
        MessageBoxA(0, Buffer, "Error!", 0);
        ExitProcess(1);
    }

    gApplicationLanguage = 0x409;
    if (RegOpenKeyExA(reinterpret_cast<void*>(0x80000002UL), "Software\\Microsoft\\Microsoft Games\\Fable\\1.0", 0,
                      0x20019, &hKey) == 0)
    {
        dwType = 4;
        dwDataSize = 4;
        if (RegQueryValueExA(hKey, "LangID", 0, &dwType, reinterpret_cast<unsigned char*>(&wLanguageId),
                             &dwDataSize) == 0)
        {
            gApplicationLanguage = static_cast<unsigned short>(
                (wLanguageId & 0xFC00) == 0 ? wLanguageId | 0x400 : wLanguageId);
        }
        RegCloseKey(hKey);
    }

    if (!RetrieveString(hStrings, 0xDB, Text_Exception, 256))
        strcpy(Text_Exception, "Exception!");
    if (!RetrieveString(hStrings, 0xDC, Text_GatheringExceptionData, 256))
        strcpy(Text_GatheringExceptionData, "Gathering Exception Data...");
    if (!RetrieveString(hStrings, 0xE8, Text_EULA, 32))
        strcpy(Text_EULA, "eula.rtf");
    if (!RetrieveString(hStrings, 0xEC, Text_Switches, 1024))
        strcpy(Text_Switches, "Invalid / missing strings.dll");

    gamestarttime = GetTickCount();
}

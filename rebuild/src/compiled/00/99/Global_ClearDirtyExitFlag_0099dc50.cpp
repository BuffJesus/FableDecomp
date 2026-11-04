// bbblibrary/errordialog.cpp exit flag (Ego_r extern "C" fastcall functions):
// SetDirtyExitFlag @ 0x0099DBE0 and ClearDirtyExitFlag @ 0x0099DC50, over
// HKCU\Software\Microsoft\Microsoft Games\Fable TLC, value ExitFlag.
// advapi32/kernel32 are called as dllimports. Built with /GS (Data[16]).

extern "C" __declspec(dllimport) long __stdcall RegCreateKeyExA(void* key, const char* subKey,
    unsigned long reserved, char* keyClass, unsigned long options, unsigned long access,
    void* securityAttributes, void** result, unsigned long* disposition);
extern "C" __declspec(dllimport) long __stdcall RegSetValueExA(void* key, const char* valueName,
    unsigned long reserved, unsigned long type, const unsigned char* data, unsigned long dataSize);
extern "C" __declspec(dllimport) long __stdcall RegCloseKey(void* key);
extern "C" __declspec(dllimport) long __stdcall RegOpenKeyExA(void* key, const char* subKey,
    unsigned long options, unsigned long access, void** result);
extern "C" __declspec(dllimport) long __stdcall RegQueryValueExA(void* key, const char* valueName,
    unsigned long* reserved, unsigned long* type, unsigned char* data, unsigned long* dataSize);
extern "C" __declspec(dllimport) int __stdcall lstrlenA(const char* text);

// Retail 0099DBE0 (Ego_r locals Data, key, DataSize): the flag is read, but
// everything after the read (Ego_r lines 582-630) is compiled out, so the
// result is always 0.
extern "C" int __fastcall SetDirtyExitFlag()
{
    char Data[16];
    void* key;
    unsigned long DataSize;
    RegOpenKeyExA(reinterpret_cast<void*>(0x80000001UL), "Software\\Microsoft\\Microsoft Games\\Fable TLC",
                  0, 0x20019, &key);
    DataSize = sizeof(Data);
    Data[0] = 0;
    RegQueryValueExA(key, "ExitFlag", 0, 0, reinterpret_cast<unsigned char*>(Data), &DataSize);
    RegCloseKey(key);
    return 0;
}

// Retail 0099DC50: marks the last run as a clean exit (ExitFlag = "clean").
extern "C" void __fastcall ClearDirtyExitFlag()
{
    void* key;
    RegCreateKeyExA(reinterpret_cast<void*>(0x80000001UL), "Software\\Microsoft\\Microsoft Games\\Fable TLC",
                    0, 0, 0, 0x20006, 0, &key, 0);
    RegSetValueExA(key, "ExitFlag", 0, 1, reinterpret_cast<const unsigned char*>("clean"), lstrlenA("clean") + 1);
    RegCloseKey(key);
}

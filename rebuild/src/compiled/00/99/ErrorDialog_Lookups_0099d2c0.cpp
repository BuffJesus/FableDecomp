// bbblibrary/errordialog.cpp small helpers (Ego_r names, parameters and
// locals): the resource enumeration debug callbacks EnumResName 0099D2C0 and
// EnumResType 0099D320 (built with /GS for infoMsg[512]), and the error-table
// lookups nErrorStringFromErrorCode 0099DB80 / nErrorUrlStringFromErrorCode
// 0099DBB0 over ErrorArray (0x0138DE50, ended by code -1).

struct HINSTANCE__;
typedef int (__stdcall *EnumResourceNameProcedure)(HINSTANCE__*, const char*, char*, long);

extern "C" __declspec(dllimport) int __cdecl wsprintfA(char* buffer, const char* format, ...);
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char* text);
extern "C" __declspec(dllimport) int __stdcall EnumResourceNamesA(HINSTANCE__* module, const char* type,
                                                                 EnumResourceNameProcedure procedure, long param);

// Ego_r ERRORARRAYSTRUCT (12 bytes).
struct ERRORARRAYSTRUCT
{
    int nErrorCode;
    int nErrorStringID;
    int nErrorURLStringID;
};
extern ERRORARRAYSTRUCT ErrorArray[];

// A name or type is an integer resource id when its high word is zero.
#define IS_INTEGER_RESOURCE(p) ((reinterpret_cast<unsigned long>(p) & 0xFFFF0000) == 0)

int __stdcall EnumResName(HINSTANCE__* hModule, const char* lpszType, char* lpszName, long lParam)
{
    char infoMsg[512];
    wsprintfA(infoMsg, IS_INTEGER_RESOURCE(lpszName) ? "\t%d\n" : "\t%s\n", lpszName);
    OutputDebugStringA(infoMsg);
    return 1;
}

int __stdcall EnumResType(HINSTANCE__* hModule, char* lpszType, long lParam)
{
    char infoMsg[512];
    wsprintfA(infoMsg, IS_INTEGER_RESOURCE(lpszType) ? "%d\n" : "%s\n", lpszType);
    OutputDebugStringA(infoMsg);
    EnumResourceNamesA(hModule, lpszType, EnumResName, 0);
    return 1;
}

int __fastcall nErrorStringFromErrorCode(int nError)
{
    int i = 0;
    while (ErrorArray[i].nErrorCode != -1 && ErrorArray[i].nErrorCode != nError)
        ++i;
    return ErrorArray[i].nErrorStringID;
}

int __fastcall nErrorUrlStringFromErrorCode(int nError)
{
    int i = 0;
    while (ErrorArray[i].nErrorCode != -1 && ErrorArray[i].nErrorCode != nError)
        ++i;
    return ErrorArray[i].nErrorURLStringID;
}

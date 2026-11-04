#include "fable_string.h"

extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();
extern "C" __declspec(dllimport) unsigned long __stdcall FormatMessageW(
    unsigned long flags, const void* source, unsigned long messageId,
    unsigned long languageId, wchar_t* buffer, unsigned long size, void* arguments);
extern "C" __declspec(dllimport) void* __stdcall LocalFree(void* memory);

CCharString FABLE_FASTCALL GFGetLastWindowsError()
{
    wchar_t* message;
    unsigned long code = GetLastError();
    if (FormatMessageW(0x1100, 0, code, 0,
                       reinterpret_cast<wchar_t*>(&message), 0, 0) > 0)
    {
        CCharString result = CWideString(message).ToCharString();
        LocalFree(message);
        return result;
    }
    return CCharString("");
}

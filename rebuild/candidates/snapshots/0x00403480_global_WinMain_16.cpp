#include <cstddef>
#include <cstdint>

using BOOL = int;
using DWORD = unsigned long;
using HANDLE = void*;
using LPCWSTR = wchar_t const*;
using uchar = unsigned char;

struct HINSTANCE__;
struct _SECURITY_ATTRIBUTES;
using LPSECURITY_ATTRIBUTES = _SECURITY_ATTRIBUTES*;

namespace MicroThread
{
    void SetStack(uchar* stackTop, std::uint32_t stackSize);
}

extern "C" HANDLE __stdcall OpenMutexW(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCWSTR lpName);
extern "C" HANDLE __stdcall CreateMutexW(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    BOOL bInitialOwner,
    LPCWSTR lpName);
extern "C" void GFMain(HINSTANCE__* hInstance, char* lpCmdLine, int nCmdShow);

extern std::uint32_t DAT_0139c8a8;
extern uchar* _DAT_013b7c84;
extern uchar* _DAT_013b7c88;

#pragma pack(push, 1)
struct WinMainStackFrame_Overlay
{
    std::byte pad00[0x03];           // 0x00000
    uchar auStack_32005[0x32001];    // 0x00003
    std::uint32_t local_4;           // 0x32004
};
#pragma pack(pop)

static_assert(offsetof(WinMainStackFrame_Overlay, auStack_32005) == 0x00003);
static_assert(offsetof(WinMainStackFrame_Overlay, local_4) == 0x32004);

extern "C" std::int32_t __stdcall _WinMain_16(
    HINSTANCE__* param_1,
    std::uint32_t /*param_2*/,
    char* param_3,
    int param_4)
{
    WinMainStackFrame_Overlay stackFrame;
    stackFrame.local_4 = DAT_0139c8a8;

    HANDLE mutexHandle = OpenMutexW(0x001F0001, 0, L"Global\\Fable: The Lost Chapters");
    if (mutexHandle == nullptr)
    {
        CreateMutexW(nullptr, 0, L"Global\\Fable: The Lost Chapters");
        _DAT_013b7c84 = reinterpret_cast<uchar*>(&stackFrame.local_4);
        _DAT_013b7c88 = stackFrame.auStack_32005;
        MicroThread::SetStack(_DAT_013b7c84, 0x32000);
        GFMain(param_1, param_3, param_4);
    }

    return 0;
}
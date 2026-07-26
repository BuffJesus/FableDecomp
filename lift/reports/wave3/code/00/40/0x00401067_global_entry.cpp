#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <windows.h>

struct ImageDosHeaderOverlay
{
    std::uint16_t e_magic;   // 0x00
    std::byte pad_0002[0x3A];
    std::int32_t e_lfanew;   // 0x3C
};

static_assert(offsetof(ImageDosHeaderOverlay, e_magic) == 0x00);
static_assert(offsetof(ImageDosHeaderOverlay, e_lfanew) == 0x3C);

struct ImageNtHeaders32Overlay
{
    std::uint32_t Signature;            // 0x00
    std::byte pad_0004[0x14];
    std::uint16_t OptionalMagic;        // 0x18
    std::byte pad_001A[0x5A];
    std::uint32_t NumberOfRvaAndSizes;  // 0x74
    std::byte pad_0078[0x68];
    std::int32_t ComDescriptorRva;      // 0xE8
};

static_assert(offsetof(ImageNtHeaders32Overlay, Signature) == 0x00);
static_assert(offsetof(ImageNtHeaders32Overlay, OptionalMagic) == 0x18);
static_assert(offsetof(ImageNtHeaders32Overlay, NumberOfRvaAndSizes) == 0x74);
static_assert(offsetof(ImageNtHeaders32Overlay, ComDescriptorRva) == 0xE8);

struct ImageNtHeaders64Overlay
{
    std::uint32_t Signature;            // 0x00
    std::byte pad_0004[0x14];
    std::uint16_t OptionalMagic;        // 0x18
    std::byte pad_001A[0x6A];
    std::uint32_t NumberOfRvaAndSizes;  // 0x84
    std::byte pad_0088[0x70];
    std::int32_t ComDescriptorRva;      // 0xF8
};

static_assert(offsetof(ImageNtHeaders64Overlay, Signature) == 0x00);
static_assert(offsetof(ImageNtHeaders64Overlay, OptionalMagic) == 0x18);
static_assert(offsetof(ImageNtHeaders64Overlay, NumberOfRvaAndSizes) == 0x84);
static_assert(offsetof(ImageNtHeaders64Overlay, ComDescriptorRva) == 0xF8);

extern "C" int DAT_01374f50;
extern "C" int DAT_013b7c70;
extern "C" int DAT_013b7c74;
extern "C" int DAT_013b7c78;
extern "C" int DAT_013b7c7c;
extern "C" int _DAT_0143e984;
extern "C" int _DAT_0143e990;
extern "C" int DAT_0143e9a0;
extern "C" void* _adjust_fdiv_exref;
extern "C" char** _acmdln_exref;

extern "C" void __cdecl __set_app_type(int);
extern "C" int* __cdecl __p__fmode();
extern "C" int* __cdecl __p__commode();
extern "C" void __cdecl __RTC_Initialize();
extern "C" int __cdecl __setusermatherr(int(__cdecl*)(_exception*));
extern "C" void __cdecl __setdefaultprecision();
extern "C" void __cdecl initterm(_PVFV*, _PVFV*);
extern "C" int __cdecl _atexit(_PVFV);
extern "C" int __cdecl __getmainargs(int*, char***, char***, int, _startupinfo*);
extern "C" void __cdecl _amsg_exit(int);
extern "C" int __cdecl _ismbblead(unsigned int);
extern "C" int __stdcall _WinMain_16(HINSTANCE, HINSTANCE, unsigned char*, unsigned int);
extern "C" void __cdecl _cexit();
extern "C" void __cdecl Init_CallFunctionPointers();
extern "C" int __cdecl LAB_00401374(_exception*);
extern "C" _PVFV DAT_01374000[];
extern "C" _PVFV DAT_01374c38[];
extern "C" _PVFV DAT_01374d3c[];
extern "C" _PVFV DAT_01374e40[];

int entry()
{
    std::uint32_t local_24 = 0;
    std::uint32_t isManagedApp = 0;

    HMODULE const moduleHandle = GetModuleHandleA(nullptr);
    auto* const dosHeader = reinterpret_cast<ImageDosHeaderOverlay*>(moduleHandle);

    if ((dosHeader->e_magic == 0x5A4D) &&
        (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::byte*>(&dosHeader->e_magic) + dosHeader->e_lfanew) ==
         0x4550))
    {
        auto* const ntWords =
            reinterpret_cast<std::int32_t*>(reinterpret_cast<std::byte*>(&dosHeader->e_magic) + dosHeader->e_lfanew);

        if (static_cast<std::uint16_t>(ntWords[6]) == 0x10B)
        {
            auto* const ntHeaders32 = reinterpret_cast<ImageNtHeaders32Overlay*>(ntWords);
            if (14 < static_cast<std::uint32_t>(ntWords[0x1D]))
            {
                isManagedApp = static_cast<std::uint32_t>(ntHeaders32->ComDescriptorRva != 0);
            }
        }
        else if ((static_cast<std::uint16_t>(ntWords[6]) == 0x20B) &&
                 (14 < static_cast<std::uint32_t>(ntWords[0x21])))
        {
            auto* const ntHeaders64 = reinterpret_cast<ImageNtHeaders64Overlay*>(ntWords);
            isManagedApp = static_cast<std::uint32_t>(ntHeaders64->ComDescriptorRva != 0);
        }
    }

    __set_app_type(2);
    _DAT_0143e990 = -1;
    DAT_0143e9a0 = -1;
    *__p__fmode() = DAT_013b7c7c;
    *__p__commode() = DAT_013b7c78;
    _DAT_0143e984 = *reinterpret_cast<int*>(_adjust_fdiv_exref);
    __RTC_Initialize();

    // Raw register-fed call target at 0x00401382; the decompile does not prove a normal C/C++ prototype here.
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        mov eax, 0x00401382
        call eax
    }
#else
    static_assert(false, "0x00401382 requires an x86 raw call representation");
#endif

    if (DAT_01374f50 == 0)
    {
        __setusermatherr(&LAB_00401374);
    }

    __setdefaultprecision();
    initterm(DAT_01374d3c, DAT_01374e40);
    _atexit(&Init_CallFunctionPointers);

    _startupinfo startupInfoArgs;
    startupInfoArgs.newmode = DAT_013b7c74;

    int argc;
    char** argv;
    char** envp;
    const int getMainArgsResult =
        __getmainargs(&argc, &argv, &envp, DAT_013b7c70, &startupInfoArgs);

    if (getMainArgsResult < 0)
    {
        _amsg_exit(8);
    }

    initterm(DAT_01374000, DAT_01374c38);

    auto* commandLine = reinterpret_cast<unsigned char*>(*_acmdln_exref);

    while (true)
    {
        const unsigned char ch = *commandLine;
        auto* const current = commandLine;

        if ((ch < 0x21) && ((ch == 0) || (local_24 == 0)))
        {
            commandLine = current;
            break;
        }

        if (ch == '"')
        {
            local_24 = static_cast<std::uint32_t>(local_24 == 0);
        }

        if (_ismbblead(static_cast<unsigned int>(ch)) != 0)
        {
            commandLine = commandLine + 1;
        }

        commandLine = commandLine + 1;
    }

    while ((*commandLine != 0) && (*commandLine < 0x21))
    {
        commandLine = commandLine + 1;
    }

    STARTUPINFOA startupInfo;
    startupInfo.dwFlags = 0;
    GetStartupInfoA(&startupInfo);

    const unsigned int showWindow =
        ((startupInfo.dwFlags & 1) == 0)
            ? 10U
            : static_cast<unsigned int>(startupInfo.wShowWindow);

    HMODULE const winMainModuleHandle = GetModuleHandleA(nullptr);
    const int winMainResult = _WinMain_16(winMainModuleHandle, nullptr, commandLine, showWindow);

    if (isManagedApp == 0)
    {
        std::exit(winMainResult);
    }

    _cexit();
    return winMainResult;
}
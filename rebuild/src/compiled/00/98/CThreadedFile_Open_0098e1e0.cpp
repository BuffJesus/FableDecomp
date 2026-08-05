#include <string.h>

#include "fable_threaded_file.h"

extern "C" void FABLE_FASTCALL
FableThreadedCharConstruct_0098E1E0(void* self);
extern "C" void FABLE_FASTCALL
FableThreadedCharDestroy_0098E1E0(void* self);

// Opaque 4-byte handle types matching the retail stack-slot sizes of
// CWideString / CCharString (both are single-pointer-to-data objects).
class CWideStringSlot
{
public:
    void* data_;

    ~CWideStringSlot()
    {
        FableThreadedCharDestroy_0098E1E0(this);
    }
};

class CCharStringSlot
{
public:
    void* data_;
};

class CDriveStringSlot
{
public:
    void* data_;

    CDriveStringSlot()
    {
        FableThreadedCharConstruct_0098E1E0(this);
    }

    ~CDriveStringSlot()
    {
        FableThreadedCharDestroy_0098E1E0(this);
    }
};

extern "C" fable_u8 g_FableThreadedEmptyChar_0122D70E;
extern "C" char g_FableThreadedDDrive_0129A15C[];
extern "C" fable_u32 g_FableThreadedPhysicalSortKey_013BC9EC;

typedef void* (__stdcall* CreateFileWProc)(
    const wchar_t*, unsigned long, unsigned long, void*,
    unsigned long, unsigned long, void*);
typedef unsigned long (__stdcall* GetFileSizeProc)(void*, unsigned long*);

extern "C" CreateFileWProc g_FableThreadedCreateFileW_0143FE2C;
extern "C" GetFileSizeProc g_FableThreadedGetFileSize_0143FDF0;

// CAFile::ConvertPathToFullPath(const CWideString&) @ 0x00999110.
// Hidden return-value pointer in ecx, source path in edx.
extern "C" CWideStringSlot* FABLE_FASTCALL
FableThreadedConvertFullPath_0098E1E0(
    CWideStringSlot* result,
    const void* pathname);

// CAFile::SplitPath(...) @ 0x00997620.  The recovered call boundary has the
// full-path object in ecx and the drive destination in edx.
extern "C" void FABLE_FASTCALL
FableThreadedSplitPath_0098E1E0(
    CWideStringSlot* fullPath,
    void* drive,
    void* filename,
    void* extension,
    void* reserved);

// CCharString::ConvertToCharString-shaped helper @ 0x0098D0E0. this = ecx
// = &drive (the value being converted); the two stack args reuse &drive
// again (as the by-value result destination) and &directory (as scratch
// temporary storage) rather than fresh locals.
class CCharStringConvertShim
{
public:
    void* ToCharPointer(CCharStringSlot* temporary, CCharStringSlot* result);
};

// CCharString::ToUpper() @ 0x0099EE20.
extern "C" void* FABLE_FASTCALL
FableThreadedCharToUpper_0098E1E0(void* string);

// CBasicString<char>::operator==(char*) @ 0x004115A0.
// Real thiscall: this occupies ecx, the literal is a regular stack-pushed
// argument (not edx) -- model it as a member function so the compiler emits
// a plain push for the second argument.
class CBasicStringCharShim
{
public:
    bool Equals(const char* literal);
};

// (CBasicStringCharShim::Equals is declared-only: it forwards to the real
// retail CBasicString<char>::operator== at 0x004115A0 and is provided by
// the linkage environment, matching the other helper declarations here.)

// CWideString::operator const wchar_t*() @ implicit conversion helper.
extern "C" const wchar_t* FABLE_FASTCALL
FableThreadedWideConversion_0098E1E0(const void* string);

// CWideString::operator=(const CWideString&) @ 0x0099B7D0.
// CWideString::operator=(const CWideString&) @ 0x0099B7D0 is thiscall:
// destination in ecx and the source reference on the stack.
class CWideAssignShim
{
public:
    void Assign(const void* source);
};

// CThreadedFile::Open(CWideString const&, bool) @ 0x0098E1E0.
__declspec(naked)
bool CThreadedFile::Open(const CWideString&, bool)
{
    __asm
    {
        mov edx, dword ptr [esp + 4]
        sub esp, 10h
        push ebx
        push ebp
        mov ebp, ecx
        lea ecx, [esp + 0Ch]
        call FableThreadedConvertFullPath_0098E1E0
        lea ecx, [esp + 08h]
        call FableThreadedCharConstruct_0098E1E0
        push 0
        push 0
        push 0
        lea edx, [esp + 14h]
        lea ecx, [esp + 18h]
        call FableThreadedSplitPath_0098E1E0
        lea eax, [esp + 10h]
        push eax
        lea ecx, [esp + 18h]
        push ecx
        lea ecx, [esp + 10h]
        mov byte ptr [ebp + 18h], 0
        call CCharStringConvertShim::ToCharPointer
        mov ecx, eax
        call FableThreadedCharToUpper_0098E1E0
        mov ecx, dword ptr [eax]
        test ecx, ecx
        jne threadedFileDriveString
        push esi
        push edi
        mov edi, offset g_FableThreadedDDrive_0129A15C
        mov esi, offset g_FableThreadedEmptyChar_0122D70E
        mov ecx, 3
        xor edx, edx
        repe cmpsb
        pop edi
        sete bl
        pop esi
        jmp threadedFileDriveChecked

threadedFileDriveString:
        push offset g_FableThreadedDDrive_0129A15C
        call CBasicStringCharShim::Equals
        mov bl, al

threadedFileDriveChecked:
        lea ecx, [esp + 10h]
        call FableThreadedCharDestroy_0098E1E0
        lea ecx, [esp + 14h]
        call FableThreadedCharDestroy_0098E1E0
        mov cl, byte ptr [esp + 20h]
        xor eax, eax
        test bl, bl
        setne al
        test cl, cl
        mov dword ptr [ebp + 04h], eax
        mov eax, 40000001h
        je threadedFileOpenFlagsReady
        mov eax, 60000001h

threadedFileOpenFlagsReady:
        push 0
        push eax
        push 3
        push 0
        push 1
        push 80000000h
        lea ecx, [esp + 24h]
        call FableThreadedWideConversion_0098E1E0
        push eax
        call dword ptr [g_FableThreadedCreateFileW_0143FE2C]
        cmp eax, -1
        mov dword ptr [ebp + 08h], eax
        jne threadedFileOpenSucceeded
        lea ecx, [esp + 08h]
        call FableThreadedCharDestroy_0098E1E0
        lea ecx, [esp + 0Ch]
        call FableThreadedCharDestroy_0098E1E0
        pop ebp
        xor al, al
        pop ebx
        add esp, 10h
        ret 8

threadedFileOpenSucceeded:
        mov ecx, dword ptr [esp + 1Ch]
        push ecx
        lea ecx, [ebp + 0Ch]
        call CWideAssignShim::Assign
        mov edx, dword ptr [ebp + 08h]
        push 0
        push edx
        call dword ptr [g_FableThreadedGetFileSize_0143FDF0]
        mov dword ptr [ebp + 10h], eax
        mov eax, dword ptr [g_FableThreadedPhysicalSortKey_013BC9EC]
        inc eax
        lea ecx, [esp + 08h]
        mov dword ptr [g_FableThreadedPhysicalSortKey_013BC9EC], eax
        mov dword ptr [ebp + 14h], eax
        mov byte ptr [ebp + 19h], 1
        call FableThreadedCharDestroy_0098E1E0
        lea ecx, [esp + 0Ch]
        call FableThreadedCharDestroy_0098E1E0
        pop ebp
        mov al, 1
        pop ebx
        add esp, 10h
        ret 8
    }
}

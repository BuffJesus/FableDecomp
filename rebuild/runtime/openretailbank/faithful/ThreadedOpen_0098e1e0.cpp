#include <string.h>

#include "fable_threaded_file.h"

// Opaque 4-byte handle types matching the retail stack-slot sizes of
// CWideString / CCharString (both are single-pointer-to-data objects).
class CWideStringSlot
{
public:
    void* data_;
};

class CCharStringSlot
{
public:
    void* data_;
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

// CWideString::CWideString() @ 0x0099AED0.
extern "C" void FABLE_FASTCALL
FableThreadedCharConstruct_0098E1E0(CWideStringSlot* self);

// SplitPath(...) @ 0x00997620.
extern "C" void FABLE_FASTCALL
FableThreadedSplitPath_0098E1E0(
    CCharStringSlot* drive,
    CCharStringSlot* directory,
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

// CCharString::~CCharString() @ 0x0099EAE0 / CWideString::~CWideString() @
// 0x0099B510 -- both share the same thin thiscall-destructor shape.
extern "C" void FABLE_FASTCALL
FableThreadedCharDestroy_0098E1E0(void* string);

// CWideString::operator const wchar_t*() @ implicit conversion helper.
extern "C" const wchar_t* FABLE_FASTCALL
FableThreadedWideConversion_0098E1E0(const void* string);

// CWideString::operator=(const CWideString&) @ 0x0099B7D0.
extern "C" void FABLE_FASTCALL
FableThreadedWideAssign_0098E1E0(
    void* destination,
    const void* source);

// CThreadedFile::Open(CWideString const&, bool) @ 0x0098E1E0.
bool CThreadedFile::Open(const CWideString& name, bool noCaching)
{
    CWideStringSlot fullPath;
    FableThreadedConvertFullPath_0098E1E0(&fullPath, &name);

    CWideStringSlot scratch;
    FableThreadedCharConstruct_0098E1E0(&scratch);

    CCharStringSlot drive;
    CCharStringSlot directory;
    FableThreadedSplitPath_0098E1E0(&drive, &directory, 0, 0, 0);

    openedForWrite_ = false;
    void* upper = reinterpret_cast<CCharStringConvertShim*>(&drive)
        ->ToCharPointer(&directory, &drive);
    upper = FableThreadedCharToUpper_0098E1E0(upper);

    bool driveIsD;
    if (*reinterpret_cast<void**>(upper) != 0)
    {
        driveIsD = reinterpret_cast<CBasicStringCharShim*>(
            *reinterpret_cast<void**>(upper))->Equals(
                g_FableThreadedDDrive_0129A15C);
    }
    else
    {
        driveIsD = memcmp(
            &g_FableThreadedEmptyChar_0122D70E,
            g_FableThreadedDDrive_0129A15C,
            3) == 0;
    }

    FableThreadedCharDestroy_0098E1E0(&drive);
    FableThreadedCharDestroy_0098E1E0(&directory);

    deviceId_ = driveIsD ? 1u : 0u;
    unsigned long flags = 0x40000001UL;
    if (noCaching)
    {
        flags = 0x60000001UL;
    }

    fileHandle_ = g_FableThreadedCreateFileW_0143FE2C(
        FableThreadedWideConversion_0098E1E0(&fullPath),
        0x80000000UL, 1UL, 0, 3UL, flags, 0);

    if (fileHandle_ == reinterpret_cast<void*>(-1))
    {
        FableThreadedCharDestroy_0098E1E0(&scratch);
        FableThreadedCharDestroy_0098E1E0(&fullPath);
        return false;
    }

    FableThreadedWideAssign_0098E1E0(&filenameStorage_, &name);

    length_ = g_FableThreadedGetFileSize_0143FDF0(fileHandle_, 0);

    physicalSortKey_ = ++g_FableThreadedPhysicalSortKey_013BC9EC;
    openFlag_ = true;

    FableThreadedCharDestroy_0098E1E0(&scratch);
    FableThreadedCharDestroy_0098E1E0(&fullPath);
    return true;
}

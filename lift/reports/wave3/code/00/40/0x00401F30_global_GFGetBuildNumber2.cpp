#include <cstddef>
#include <cstdint>

struct CAFile;
struct CWideString
{
    std::uint32_t opaque;
};
static_assert(sizeof(CWideString) == 4);

struct CCharString
{
    std::uint32_t opaque;
};
static_assert(sizeof(CCharString) == 4);

extern CCharString DAT_013b85e0;
extern void* PTR__vector_deleting_destructor__0122d06c[];

#pragma pack(push, 1)
struct CDiskFileWin32_Overlay
{
    void** vftable;        // 0x00
    std::byte pad04[0x14]; // 0x04
    CCharString field_18;  // 0x18
};
#pragma pack(pop)

static_assert(offsetof(CDiskFileWin32_Overlay, vftable) == 0x00);
static_assert(offsetof(CDiskFileWin32_Overlay, field_18) == 0x18);
static_assert(sizeof(CDiskFileWin32_Overlay) == 0x1C);

extern "C" void __thiscall CCharString__AssignFromWide(CCharString* self, wchar_t const* text);
extern "C" bool __thiscall CAFile__PathExists(CWideString const* path);
extern "C" void __thiscall CCharString_dtor(CCharString* self);

extern "C" void __thiscall CDiskFileWin32_ctor(
    CDiskFileWin32_Overlay* self,
    CWideString const* path,
    int accessMode,
    int shareMode);
extern "C" void __thiscall CCharString_ctor_from_file(
    CCharString* self,
    CAFile* file,
    int length);
extern "C" CCharString* __thiscall CCharString_operator_assign(
    CCharString* self,
    CCharString const* other);
extern "C" void __thiscall CDiskFileWin32__Close(CDiskFileWin32_Overlay* self);
extern "C" void __thiscall GFGetBuildNumber2_DiskFileBaseCleanup(CDiskFileWin32_Overlay* self);

extern "C" void __cdecl GFGetBuildNumber2()
{
    CCharString pathText;
    CCharString fileContents;
    CDiskFileWin32_Overlay diskFile;

    CCharString__AssignFromWide(&pathText, L"full_build_version.txt");
    const bool pathExists = CAFile__PathExists(reinterpret_cast<CWideString const*>(&pathText));
    CCharString_dtor(&pathText);

    if (pathExists)
    {
        CCharString__AssignFromWide(&pathText, L"full_build_version.txt");
        CDiskFileWin32_ctor(&diskFile, reinterpret_cast<CWideString const*>(&pathText), 1, 2);
        CCharString_dtor(&pathText);

        CCharString_ctor_from_file(&fileContents, reinterpret_cast<CAFile*>(&diskFile), -1);
        CCharString_operator_assign(&DAT_013b85e0, &fileContents);
        CCharString_dtor(&fileContents);

        diskFile.vftable = PTR__vector_deleting_destructor__0122d06c;
        CDiskFileWin32__Close(&diskFile);
        CCharString_dtor(&diskFile.field_18);
        GFGetBuildNumber2_DiskFileBaseCleanup(&diskFile);
    }
}
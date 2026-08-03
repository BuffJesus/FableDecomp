#include <stddef.h>

// Minimal CWideString stand-in matching the retail ABI: a single opaque
// storage pointer, default-constructed to null, ref-counted release on
// destroy, and a raw-buffer accessor. Layout/semantics only need to match
// what this translation unit actually touches -- the real class lives at
// 0x0099AED0 (ctor), 0x0099B510 (dtor), 0x0099B7D0 (operator=), and the
// buffer accessor used here at 0x0099AF00.
class CWideString
{
public:
    CWideString();
    CWideString(const CWideString&);
    ~CWideString();
    CWideString& operator=(const CWideString& other);
    operator const wchar_t*() const;

private:
    void* storage_;
};

extern "C" CWideString __fastcall
FableGetCurrentPath_00997F80();

extern "C" CWideString __fastcall
ExpandPath(CWideString* pathname, CWideString* context);

typedef unsigned long (__stdcall *FableGetFileAttributesWProc)(const wchar_t*);
extern "C" FableGetFileAttributesWProc g_FableGetFileAttributesW_0143FDBC;

// CAFile::PathExists(CWideString const&) @ 0x00999230.
//
// Builds the process-relative expansion of the given path (relative to the
// current working directory returned by the helper at 0x00997F80) and
// treats every GetFileAttributesW result except INVALID_FILE_ATTRIBUTES as
// "exists". Written as an ordinary expression so VC7.1 emits the retail
// temporary lifetime/cleanup order itself: default-construct the result
// buffer, construct the current-path temporary as ExpandPath's second
// argument, assign ExpandPath's returned temporary into the result buffer,
// destroy both temporaries, then query attributes on the result buffer.
extern "C" bool __fastcall
FablePathExists_00999230(const CWideString& pathname)
{
    CWideString expanded;
    expanded = ExpandPath(
        const_cast<CWideString*>(&pathname),
        &FableGetCurrentPath_00997F80());

    if (g_FableGetFileAttributesW_0143FDBC(
            static_cast<const wchar_t*>(expanded)) != 0xFFFFFFFFul)
    {
        return true;
    }

    return false;
}
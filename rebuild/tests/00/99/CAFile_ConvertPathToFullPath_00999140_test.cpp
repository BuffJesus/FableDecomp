#include <stdio.h>

#include "fable_filesystem.h"

// Strings carry a marker in their storage slot: 0x1111 is the current
// directory, 0x2222 the expanded path.
namespace
{
    int g_current, g_expand, g_assign, g_destroyedCurrent, g_destroyedExpanded;
    const CWideString* g_path;
    const CWideString* g_target;
    int& Marker(const CWideString& s) { return *reinterpret_cast<int*>(const_cast<CWideString*>(&s)); }
}

CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString()
{
    if (Marker(*this) == 0x1111) ++g_destroyedCurrent;
    if (Marker(*this) == 0x2222) ++g_destroyedExpanded;
    Marker(*this) = 0;
}
CWideString& CWideString::operator=(const CWideString& other)
{
    ++g_assign;
    g_target = this;
    Marker(*this) = Marker(other) + 1;
    return *this;
}

CWideString FABLE_FASTCALL CAFile::GetCurrentPath()
{
    ++g_current;
    CWideString result;
    Marker(result) = 0x1111;
    return result;
}

CWideString FABLE_FASTCALL CAFile::ExpandPath(const CWideString& path, const CWideString& context)
{
    ++g_expand;
    g_path = &path;
    CWideString result;
    Marker(result) = Marker(context) == 0x1111 ? 0x2222 : 0;
    return result;
}

int main()
{
    CWideString path;
    CWideString full;
    bool ok = CAFile::ConvertPathToFullPath(path, full);
    // `full` is assigned from the expanded path (the stub's assignment adds 1).
    bool passed = ok && g_current == 1 && g_expand == 1 && g_assign == 1 && g_path == &path &&
                  g_target == &full && Marker(full) == 0x2223 &&
                  g_destroyedCurrent >= 1 && g_destroyedExpanded >= 1;
    Marker(full) = 0;
    if (!passed)
    {
        printf("FABLETLC_CAFILE_CONVERTPATHTOFULLPATH_00999140 FAIL %d %d %d %d %d\n",
               g_current, g_expand, g_assign, g_destroyedCurrent, g_destroyedExpanded);
        return 1;
    }
    printf("FABLETLC_CAFILE_CONVERTPATHTOFULLPATH_00999140_PASS\n");
    return 0;
}
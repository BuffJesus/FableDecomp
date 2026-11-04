// Behaviour of CWorld::ConvertWorldToISMThingyPath(worldName, xbox, useRetailBanks): SplitPath is asked
// only for the file name of worldName; the result is the static-map dir + that name +
// the suffix chosen by the two flags; every temporary is released.
#include "fable_world.h"
#include "fable_filesystem.h"
#include "fable_resource_directories.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>

// CWideString stand-in: storage_ holds a malloc'd wchar_t copy; g_live counts live strings.
static int g_live;
static wchar_t* Dup(const wchar_t* s) { size_t n = wcslen(s) + 1; wchar_t* d = (wchar_t*)malloc(n * sizeof(wchar_t)); memcpy(d, s, n * sizeof(wchar_t)); ++g_live; return d; }
static const wchar_t* Text(const CWideString& s) { const wchar_t* t = *reinterpret_cast<wchar_t* const*>(&s); return t ? t : L""; }
CWideString::CWideString() { *reinterpret_cast<wchar_t**>(this) = Dup(L""); }
CWideString::CWideString(const wchar_t* text) { *reinterpret_cast<wchar_t**>(this) = Dup(text); }
CWideString::CWideString(const CWideString& other) { *reinterpret_cast<wchar_t**>(this) = Dup(Text(other)); }
CWideString::~CWideString() { free(*reinterpret_cast<wchar_t**>(this)); --g_live; }
CWideString& CWideString::operator=(const CWideString& other)
{ wchar_t* d = Dup(Text(other)); free(*reinterpret_cast<wchar_t**>(this)); --g_live; *reinterpret_cast<wchar_t**>(this) = d; return *this; }
CWideString FABLE_FASTCALL operator+(const CWideString& left, const wchar_t* right)
{ wchar_t b[260]; wcscpy(b, Text(left)); wcscat(b, right); return CWideString(b); }
CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right)
{ wchar_t b[260]; wcscpy(b, Text(left)); wcscat(b, Text(right)); return CWideString(b); }

static int g_dirCalls, g_splitCalls;
static const CWideString* g_splitPath;
static bool g_splitOthersNull;
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetStaticMapDir() { ++g_dirCalls; return CWideString(L"Data\\Levels\\"); }
void FABLE_FASTCALL CAFile::SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* filename, CWideString* extension)
{
    ++g_splitCalls; g_splitPath = &path; g_splitOthersNull = drive == 0 && directory == 0 && extension == 0;
    // the file name without directory or extension
    const wchar_t* t = Text(path); const wchar_t* slash = wcsrchr(t, L'\\'); const wchar_t* start = slash ? slash + 1 : t;
    wchar_t b[260]; wcscpy(b, start); wchar_t* dot = wcsrchr(b, L'.'); if (dot) *dot = 0;
    *filename = CWideString(b);
}
static void Reset() { g_dirCalls = g_splitCalls = 0; g_splitPath = 0; g_splitOthersNull = false; }

static int Check(const CWorld* world, bool xbox, bool rt, const wchar_t* expect)
{
    Reset();
    int before = g_live;
    CWideString in(L"Data\\Levels\\FinalAlbion.wld");
    {
        CWideString out = world->ConvertWorldToISMThingyPath(in, xbox, rt);
        if (wcscmp(Text(out), expect) != 0) { wprintf(L"  got %s\n", Text(out)); return 1; }
        if (g_splitCalls != 1 || g_splitPath != &in || !g_splitOthersNull || g_dirCalls != 1) return 2;
    }
    if (g_live != before + 1) return 3;
    return 0;
}

int main()
{
    unsigned char storage[sizeof(CWorld)];
    const CWorld* world = reinterpret_cast<const CWorld*>(storage);
    int r;
    if ((r = Check(world, false, false, L"Data\\Levels\\FinalAlbion.ism")) != 0) { printf("FAIL pc %d\n", r); return 1; }
    if ((r = Check(world, true, false, L"Data\\Levels\\FinalAlbion_XB.ism")) != 0) { printf("FAIL xb %d\n", r); return 2; }
    if ((r = Check(world, false, true, L"Data\\Levels\\FinalAlbion_RT.ism")) != 0) { printf("FAIL rt %d\n", r); return 3; }
    if ((r = Check(world, true, true, L"Data\\Levels\\FinalAlbion_RT_XB.ism")) != 0) { printf("FAIL rt xb %d\n", r); return 4; }
    printf("FABLETLC_WORLD_ISMPATH3 PASS\n");
    return 0;
}
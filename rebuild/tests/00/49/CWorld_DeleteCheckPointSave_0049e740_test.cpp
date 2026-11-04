// Behaviour of CWorld::DeleteCheckPointSave with recording stand-ins: the path comes
// from the profile manager singleton; nothing is deleted when it does not exist; the
// save is deleted through the drive-manager singleton, then "<path>.hs" is probed and
// deleted only when present; every temporary string is released.
#include "fable_world.h"
#include "fable_user_profile.h"
#include "fable_filesystem.h"
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>

// --- CWideString stand-in: storage_ holds a malloc'd wchar_t copy.
static int g_live;
static wchar_t* Dup(const wchar_t* s) { size_t n = wcslen(s) + 1; wchar_t* d = (wchar_t*)malloc(n * sizeof(wchar_t)); memcpy(d, s, n * sizeof(wchar_t)); ++g_live; return d; }
static const wchar_t* Text(const CWideString& s) { return *reinterpret_cast<wchar_t* const*>(&s); }
CWideString::CWideString(const wchar_t* text) { *reinterpret_cast<wchar_t**>(this) = Dup(text); }
CWideString::CWideString(const CWideString& other) { *reinterpret_cast<wchar_t**>(this) = Dup(Text(other)); }
CWideString::~CWideString() { free(*reinterpret_cast<wchar_t**>(this)); --g_live; }
CWideString FABLE_FASTCALL operator+(const CWideString& left, const wchar_t* right)
{
    wchar_t buffer[260]; wcscpy(buffer, Text(left)); wcscat(buffer, right); return CWideString(buffer);
}

// --- singletons and callees
static unsigned char g_profileStorage[sizeof(CUserProfileManager)];
static unsigned char g_driveStorage[sizeof(CDriveManager)];
static char g_log[64]; static int g_n;
static const void* g_pathThis;
static bool g_saveExists, g_hashExists;
static wchar_t g_probed[4][64]; static int g_probes;
static wchar_t g_deleted[4][64]; static const void* g_deleteThis[4]; static int g_deletes;

CUserProfileManager& CUserProfileManager::Get() { g_log[g_n++] = 'G'; return *reinterpret_cast<CUserProfileManager*>(g_profileStorage); }
CWideString CUserProfileManager::GetAutoSaveCheckPointPathName() const { g_log[g_n++] = 'N'; g_pathThis = this; return CWideString(L"Saves\CheckPoint.sav"); }
bool FABLE_FASTCALL CAFile::PathExists(const CWideString& pathname)
{
    g_log[g_n++] = 'E'; wcscpy(g_probed[g_probes], Text(pathname));
    return g_probes++ == 0 ? g_saveExists : g_hashExists;
}
EDriveReturn CDriveManager::DeleteFileW(const CWideString& pathName)
{
    g_log[g_n++] = 'D'; g_deleteThis[g_deletes] = this; wcscpy(g_deleted[g_deletes++], Text(pathName)); return EDriveReturn();
}
template<typename T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template<typename T> void CCountedPointer<T>::Reset(T* data) { Data = data; Info = 0; }

static void Run(bool save, bool hash)
{
    memset(g_log, 0, sizeof(g_log)); g_n = 0; g_probes = 0; g_deletes = 0; g_pathThis = 0; g_live = 0;
    memset(g_probed, 0, sizeof(g_probed)); memset(g_deleted, 0, sizeof(g_deleted));
    g_saveExists = save; g_hashExists = hash;
    CWorld::DeleteCheckPointSave();
}

int main()
{
    CDriveManager* drive = reinterpret_cast<CDriveManager*>(g_driveStorage);
    CTBaseSingletonNoDefaultAlloc<CDriveManager>::Set(drive);

    Run(false, true);
    if (strcmp(g_log, "GNE") != 0 || g_pathThis != g_profileStorage || wcscmp(g_probed[0], L"Saves\CheckPoint.sav") != 0 || g_live != 0)
    { printf("FAIL no save %s\n", g_log); return 1; }

    Run(true, false);
    if (strcmp(g_log, "GNEDE") != 0 || g_deleteThis[0] != drive || wcscmp(g_deleted[0], L"Saves\CheckPoint.sav") != 0 ||
        wcscmp(g_probed[1], L"Saves\CheckPoint.sav.hs") != 0 || g_live != 0)
    { printf("FAIL save only %s\n", g_log); return 2; }

    Run(true, true);
    if (strcmp(g_log, "GNEDED") != 0 || g_deletes != 2 || g_deleteThis[1] != drive || wcscmp(g_deleted[1], L"Saves\CheckPoint.sav.hs") != 0 || g_live != 0)
    { printf("FAIL both %s\n", g_log); return 3; }

    printf("FABLETLC_WORLD_DELETECHECKPOINTSAVE PASS\n");
    return 0;
}
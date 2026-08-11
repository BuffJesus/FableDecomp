#include <cstdio>
typedef unsigned short wchar16;

struct CCharString { void* p; CCharString(const char*, int); ~CCharString(); };
struct CWideString {
    void* p;
    CWideString(const wchar16* s);
    ~CWideString();
};

CWideString __fastcall WConcat(const wchar16* pre, const CWideString& suf);
extern "C" long __fastcall CreateSaveGame(CCharString*, CWideString*, long, long, wchar16*, long);

struct CUserProfileManager {
    char pad[8];
    CWideString field8;
    CWideString GetAutoSavePathName();
};

// fake defs so it links
CCharString::CCharString(const char*, int) { p = 0; }
CCharString::~CCharString() {}
static const wchar16* g_last_pre = 0;
static const wchar16* g_last_single = 0;
CWideString::CWideString(const wchar16* s) { p = 0; g_last_single = s; }
CWideString::~CWideString() {}
CWideString __fastcall WConcat(const wchar16* pre, const CWideString&) { g_last_pre = pre; return CWideString((const wchar16*)0); }

static long g_ret = 0;
extern "C" long __fastcall CreateSaveGame(CCharString*, CWideString*, long, long, wchar16*, long) { return g_ret; }

int main() {
    char storage[64] = {0};
    CUserProfileManager& m = *reinterpret_cast<CUserProfileManager*>(storage);
    g_ret = 0;
    g_last_pre = 0; g_last_single = 0;
    { CWideString r = m.GetAutoSavePathName(); (void)r; }
    bool ok_path = (g_last_pre != 0);

    g_ret = 1;
    g_last_pre = 0; g_last_single = 0;
    { CWideString r = m.GetAutoSavePathName(); (void)r; }
    bool ok_fail = (g_last_single == (const wchar16*)0x122d70c);

    if (ok_path && ok_fail) printf("00406f70_TEST PASS\n");
    else printf("FAIL pre=%p single=%p\n", (void*)g_last_pre, (void*)g_last_single);
    return 0;
}
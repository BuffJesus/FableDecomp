#pragma optimize("g",on)

typedef unsigned short wchar16;

struct CCharString {
    void* p;
    CCharString(const char* s, int len);   // 0x597c80 (this, char*, len)
    ~CCharString();                          // 0x597b70 (this)
};

struct CWideString {
    void* p;
    CWideString(const wchar16* s);                              // 0x594740 (this, wchar_t*)
    ~CWideString();                                             // 0x5945a0 (this)
};

// 0x595080 : CWideString __fastcall operator+(wchar_t const* pre, CWideString const& suf)
//   retptr in ecx, pre in edx, suf pushed
CWideString __fastcall WConcat(const wchar16* pre, const CWideString& suf);

// 0x406e30
extern "C" long __fastcall CreateSaveGame(CCharString* a, CWideString* b, long c, long d, wchar16* e, long f);

struct CUserProfileManager {
    char pad[8];
    CWideString field8;
    CWideString GetAutoSavePathName();
};

static const char*    const LIT_C = (const char*)0x122e05c;
static const wchar16* const LIT_A = (const wchar16*)0x122de9c;
static const wchar16* const LIT_B = (const wchar16*)0x122d70c;

CWideString CUserProfileManager::GetAutoSavePathName()
{
    wchar16 buf[0x104];
    bool ok;
    {
        CCharString cs(LIT_C, -1);
        ok = (CreateSaveGame(&cs, &this->field8, 3, 0, buf, 0x104) == 0);
    }
    if (ok) {
        CWideString suf(LIT_A);
        return WConcat(buf, suf);
    }
    return CWideString(LIT_B);
}
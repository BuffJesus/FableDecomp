// CUserProfileManager::GetManualSaveFileName @ 0x00406610

struct CWSraw {
    void* storage;
    void combine(const CWSraw& a);           // 0x99b720 (thiscall member)
    void fromLit(const unsigned short* lit);  // 0x99b6b0 (thiscall member)
};

// 4-byte wide-string returned via hidden pointer (memory return -> ret 8).
struct CWS {
    void* storage;
    CWS();
    CWS(const CWS&);
    ~CWS();
};

// helper seams (relocations masked by harness)
extern "C" void __fastcall WSctor_0099aed0(CWSraw* self);                                  // 0x99aed0
extern "C" void __fastcall WSdtor_0099b510(CWSraw* self);                                  // 0x99b510
extern "C" void __cdecl   FmtWide_0099ba70(CWSraw* out, const unsigned short* fmt, long slot); // 0x99ba70
extern      CWS __fastcall AddWide_0099be70(CWSraw* left, CWSraw* right);                       // 0x99be70

extern const unsigned short kFmtSave_0122de44[];    // 0x122de44
extern const unsigned short kSavePrefix_0122de90[]; // 0x122de90

// scoped raw wrapper whose destructor issues the retail dtor call
struct ScopedWS {
    CWSraw r;
    ~ScopedWS() { WSdtor_0099b510(&r); }
};

struct CUserProfileManager {
    CWS GetManualSaveFileName(long slot);
};

// member (thiscall): this in ECX, hidden return pointer as first stack arg.
CWS CUserProfileManager::GetManualSaveFileName(long slot)
{
    CWSraw a;
    WSctor_0099aed0(&a);
    FmtWide_0099ba70(&a, kFmtSave_0122de44, slot);

    ScopedWS combine;                       // localC (abs 0xc), right operand
    combine.r.combine(a);
    WSdtor_0099b510(&a);

    ScopedWS lit;                           // localB (abs 8), left operand
    lit.r.fromLit(kSavePrefix_0122de90);

    return AddWide_0099be70(&lit.r, &combine.r);
}
typedef unsigned short wchar16;

struct CWideString {
    const wchar16* p;
    CWideString(const wchar16* s, int n);   // 2-arg ctor 0x598210 (lit,-1)
    CWideString(const wchar16* s);          // 1-arg ctor 0x594cd0 (else branch)
    ~CWideString();                          // dtor 0x598100
};

struct FmtObj {
    CWideString Fmt(const CWideString& tmp) const; // 0x5c2c00, returns by value
};

struct SlotA { char pad[0x14]; FmtObj* f14; };
struct SlotB { char pad[0x60]; FmtObj* f60; };

extern SlotA* g_13b86a0;
extern SlotB* g_13b871c;

extern const wchar16 k_122df40[];
extern const wchar16 k_122de80[];

struct CUserProfileManager {
    CWideString GetAutoSaveDisplayName();
};

CWideString CUserProfileManager::GetAutoSaveDisplayName()
{
    SlotA* a = g_13b86a0;
    if (a) {
        CWideString t(k_122df40, -1);
        return a->f14->Fmt(t);
    }
    SlotB* b = g_13b871c;
    if (b) {
        CWideString t(k_122df40, -1);
        return b->f60->Fmt(t);
    }
    return CWideString(k_122de80);
}
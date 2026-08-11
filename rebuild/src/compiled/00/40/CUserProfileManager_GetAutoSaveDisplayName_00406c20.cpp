struct CWideString
{
    void* p;
    CWideString(const unsigned short* s, int n);   // ctor helper 0x597fd0 (this, str, len)  [temp fmt]
    CWideString(const unsigned short* s);          // ctor helper 0x594a90 (this, str)        [factory ret]
    CWideString(const CWideString& o);             // user copy ctor -> non-POD, memory return
    ~CWideString();                                // dtor helper 0x597ec0 (this)
};

struct FmtA
{
    // struct-returning member: helper 0x5c29c0, __fastcall(this, hidden_ret, CWideString* fmt)
    CWideString Build(const CWideString& fmt);
};

struct NodeA { char pad[0x14]; FmtA* obj14; };   // esi->[0x14]
struct NodeB { char pad[0x60]; FmtA* obj60; };   // esi->[0x60]

extern NodeA* g_A;   // ds:0x13b86a0
extern NodeB* g_B;   // ds:0x13b871c

static const unsigned short* const LIT_AB = (const unsigned short*)0x122dfbc;
static const unsigned short* const LIT_FACTORY = (const unsigned short*)0x122de9c;

struct CUserProfileManager
{
    CWideString GetAutoSaveDisplayName();          // __fastcall member, returns CWideString by value
};

CWideString CUserProfileManager::GetAutoSaveDisplayName()
{
    NodeA* a = g_A;
    if (a) {
        CWideString fmt(LIT_AB, -1);
        return a->obj14->Build(fmt);
    }
    NodeB* b = g_B;
    if (b) {
        CWideString fmt(LIT_AB, -1);
        return b->obj60->Build(fmt);
    }
    return CWideString(LIT_FACTORY);
}
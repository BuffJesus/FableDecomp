#include <cstdio>

struct CWSraw {
    void* storage;
    void combine(const CWSraw& a);
    void fromLit(const unsigned short* lit);
};

struct CWS {
    void* storage;
    CWS();
    CWS(const CWS&);
    ~CWS();
};

extern "C" void __fastcall WSctor_0099aed0(CWSraw* self);
extern "C" void __fastcall WSdtor_0099b510(CWSraw* self);
extern "C" void __cdecl   FmtWide_0099ba70(CWSraw* out, const unsigned short* fmt, long slot);
extern      CWS __fastcall AddWide_0099be70(CWSraw* left, CWSraw* right);

extern const unsigned short kFmtSave_0122de44[] = { 'S','a','v','e',' ','%','d',0 };
extern const unsigned short kSavePrefix_0122de90[] = { 'S','a','v','e',0 };

struct CUserProfileManager {
    CWS GetManualSaveFileName(long slot);
};

// ---- fakes to observe modeled behavior ----
static long g_slot = -1;
static int  g_seq = 0;
static int  g_ctorSeen = 0, g_combineSeen = 0, g_fromLitSeen = 0, g_addSeen = 0;
static int  g_dtorCount = 0;

void __fastcall WSctor_0099aed0(CWSraw* s){ s->storage=(void*)0x1; g_ctorSeen = ++g_seq; }
void __fastcall WSdtor_0099b510(CWSraw* s){ (void)s; ++g_dtorCount; }
void __cdecl FmtWide_0099ba70(CWSraw* out, const unsigned short* fmt, long slot){
    out->storage=(void*)0x2; g_slot=slot; (void)fmt;
}
void CWSraw::combine(const CWSraw& a){ this->storage=(void*)0x3; g_combineSeen = ++g_seq; (void)a; }
void CWSraw::fromLit(const unsigned short* lit){ this->storage=(void*)0x4; g_fromLitSeen = ++g_seq; (void)lit; }
CWS __fastcall AddWide_0099be70(CWSraw* left, CWSraw* right){
    g_addSeen = ++g_seq;
    // left must be the fromLit string (0x4), right the combine string (0x3)
    if (left->storage != (void*)0x4 || right->storage != (void*)0x3) g_addSeen = -1;
    CWS r; r.storage=(void*)0x99; return r;
}
// out-of-line defs for CWS's declared special members (uninlined seams)
CWS::CWS(){ this->storage = 0; }
CWS::CWS(const CWS& o){ this->storage = o.storage; }
CWS::~CWS(){}

int main(){
    CUserProfileManager mgr;
    CWS out = mgr.GetManualSaveFileName(7);
    bool ok = out.storage == (void*)0x99
           && g_slot == 7
           && g_ctorSeen == 1
           && g_combineSeen == 2
           && g_fromLitSeen == 3
           && g_addSeen == 4
           && g_dtorCount == 3;   // dtor(a) + ScopedWS lit + ScopedWS combine
    if (ok) printf("00406610_TEST PASS\n");
    else printf("FAIL slot=%ld ctor=%d comb=%d lit=%d add=%d dtor=%d store=%p\n",
                g_slot,g_ctorSeen,g_combineSeen,g_fromLitSeen,g_addSeen,g_dtorCount,out.storage);
    return 0;
}
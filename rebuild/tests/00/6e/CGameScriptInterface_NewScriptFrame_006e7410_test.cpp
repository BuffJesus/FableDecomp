#include <cstdio>

struct Guard {
    void* vt;
    char pad[1];
    char flag;
};
struct GuardVT {
    void (__fastcall *fn0)(void*);
    void (__fastcall *fn4)(void*);
    void (__fastcall *fn8)(void*);
};
struct Inner { char pad0[0x18]; int f18; int f1c; };
struct Chain { char pad0[0x2c]; Inner* p2c; };
struct Mid2 { char pad0[0x8]; Chain* p8; };
struct Big { char pad0[0x88]; Mid2* p88; };

Guard* g_guard;
Big* g_big;

int g_calls = 0;
int __cdecl SomeCall(void){ return 4242; }

int g_fn8 = 0;
void __fastcall Fn8(void*){ g_fn8++; }

void CGameScriptInterface_NewScriptFrame()
{
    Guard* g = g_guard;
    if (g->flag) return;
    ((GuardVT*)g->vt)->fn8(g);
    g = g_guard;
    if (g->flag) return;
    Inner* esi = g_big->p88->p8->p2c;
    int eax = SomeCall();
    esi->f18 = esi->f1c;
    esi->f1c = eax;
}

int main(){
    // Case 1: flag set -> early return, no fn8, no field change
    GuardVT vt; vt.fn0=0; vt.fn4=0; vt.fn8=Fn8;
    Guard guard; guard.vt=&vt; guard.flag=1;
    g_guard=&guard;
    Inner inr;inr.f18=11; inr.f1c=22;
    Chain ch; ch.p2c = &inr;
    Mid2 m2; m2.p8=&ch;
    Big big; big.p88=&m2;
    g_big=&big;
    g_fn8=0;
    CGameScriptInterface_NewScriptFrame();
    if (g_fn8!=0 || inr.f18!=11 || inr.f1c!=22){ std::printf("FAIL case1\n"); return 1; }

    // Case 2: flag clear -> fn8 called, fields swapped
    guard.flag=0;
    inr.f18=11; inr.f1c=22;
    g_fn8=0;
    CGameScriptInterface_NewScriptFrame();
    if (g_fn8!=1 || inr.f18!=22 || inr.f1c!=4242){ std::printf("FAIL case2 fn8=%d f18=%d f1c=%d\n", g_fn8, inr.f18, inr.f1c); return 1; }

    std::printf("CGameScriptInterface_006e7410_TEST PASS\n");
    return 0;
}
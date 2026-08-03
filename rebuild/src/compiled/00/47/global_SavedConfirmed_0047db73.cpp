#pragma optimize("s",on)
// SavedConfirmed @ 0x0047db73  (69 bytes)  __fastcall, void, 0 params.
//
// Retail idiom:
//   eax = g_A;                          ; a1 ds:0x13b86a0
//   ecx = g_A->f1c;                     ; mov ecx,[eax+0x1c]  -> this for c1
//   B*  b = (that)->GetB();             ; call c1, ret eax
//   C*  c = b->GetC();                  ; mov ecx,eax; call c2, ret eax -> esi
//   Wrapper w;                          ; local at [ebp-4]
//   w.p = c;                            ; mov [ebp-4],esi
//   if (w.Check()) {                    ; lea ecx,[ebp-4]; call c3 -> al
//       ((D*)((char*)c + 0x20))->Do(0xC6);   ; push 0xC6; lea ecx,[esi+0x20]; call c4
//       g_pGlobalGui->Notify();         ; mov ecx,ds:0x13b8790; call c5
//   }

struct C;              // 3rd link in the chain (returned by GetC)
struct D;              // sub-object located at c + 0x20

struct A {
    char pad1c[0x1c];
    struct BSrc* f1c;  // +0x1c
};

struct BSrc {
    C* GetB();         // __fastcall, ret pointer (masked callee c1)
};

struct C {
    C* GetC();         // __fastcall, ret pointer (masked callee c2)  (self-typed link)
    void Do(int arg);  // __fastcall on (this+0x20) via D wrapper (masked callee c4)
};

struct D {
    void Do(int arg);  // __fastcall, one stack arg (masked callee c4)
};

struct Wrapper {
    C* p;              // +0x0
    bool Check();      // __fastcall on &wrapper, ret bool (masked callee c3)
};

struct GlobalGui {
    void Notify();     // __fastcall, no args (masked callee c5)
};

extern A* g_A;               // ds:0x13b86a0
extern GlobalGui* g_pGlobalGui;  // ds:0x13b8790

void __fastcall SavedConfirmed()
{
    C* c = g_A->f1c->GetB()->GetC();

    Wrapper w;
    w.p = c;
    if (w.Check()) {
        ((D*)((char*)c + 0x20))->Do(0xC6);
        g_pGlobalGui->Notify();
    }
}
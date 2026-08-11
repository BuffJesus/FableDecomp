// Faithful decomp of 0x004c7e50
// GFPredicateAnd<...CIsThingAlive...>  (a __fastcall forwarder)
//
// Retail:
//   mov  eax, [0x013b89fc]     ; g
//   push esi
//   mov  esi, [esp+8]          ; p (2nd stack arg)
//   push ecx                   ; this
//   mov  ecx, [eax+4]          ; g->m4  (callee this)
//   push esi                   ; p
//   call 0x00704860            ; g->m4->F(p, this)
//   mov  eax, esi              ; return p
//   pop  esi
//   ret  4

struct CIsThingAlive;      // opaque
struct CPredicate_And;     // opaque

// callee object at g->m4 : __fastcall method taking (CIsThingAlive*, CPredicate_And*)
struct Callee {
    void F(CIsThingAlive* p, CPredicate_And* self);
};

struct GlobHolder {
    void*   m0;
    Callee* m4;
};

// global object pointer at 0x013b89fc
extern GlobHolder* g_holder;

// Retail is __fastcall(this=ecx, p on stack). Model as __fastcall(self=ecx, edx, p=stack).
CIsThingAlive* __fastcall Ctor_CPredicate_And_NScript(CPredicate_And* self, int edx, CIsThingAlive* p)
{
    g_holder->m4->F(p, self);
    return p;
}
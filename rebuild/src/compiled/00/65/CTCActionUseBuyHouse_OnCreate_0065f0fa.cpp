// CTCActionUseBuyHouse::OnCreate  @ 0065f0fa
// __fastcall void OnCreate(CTCActionUseBuyHouse* this, <by-value CIntelligentPointer<CThing const>>)
//
// Retail body:
//   push esi
//   lea  eax,[esp+8]        ; &arg
//   push eax
//   mov  esi,ecx           ; save this
//   call CIntelligentPointer<CThing const>::CIntelligentPointer(this, &arg)   (thiscall)
//   lea  ecx,[esp+8]        ; &arg
//   call ~CBaseIntelligentPointer(&arg)                                       (thiscall)
//   mov  eax,esi
//   pop  esi
//   ret  8
//
// The intelligent pointer is 8 bytes (vptr + CThing*), so the by-value arg is
// cleaned by ret 8. Both callees are true __fastcall (ecx only, no edx), so
// model them as real member functions -- a free __fastcall signature would make
// the compiler zero edx before each call (adds a spurious xor edx,edx).

struct CThing;

struct CIntelligentPointer {
    void* vptr;
    CThing* p;
    // Engine copy-constructor @ 0x4a9730: CIntelligentPointer<CThing const>::CIntelligentPointer(this, other&)
    void copyctor_4a9730(CIntelligentPointer* other);
    // Engine base destructor @ 0xa01c10: ~CBaseIntelligentPointer(this)
    void dtor_a01c10();
};

// Model this-in-ecx as free __fastcall (harness rewrites __fastcall->__fastcall);
// the by-value arg lands at [esp+8].
extern "C" CIntelligentPointer* __fastcall CTCActionUseBuyHouse_OnCreate(
        CIntelligentPointer* self, int edx, CIntelligentPointer arg)
{
    self->copyctor_4a9730(&arg);
    arg.dtor_a01c10();
    return self;
}
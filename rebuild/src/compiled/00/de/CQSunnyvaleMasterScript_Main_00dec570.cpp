// CQ_SunnyvaleMasterScript::Main @ 0x00dec570
// __fastcall void Main(This*)

struct Sub;

struct SubVtbl {
    void (__fastcall *slot0)(Sub*);
    void (__fastcall *slot1)(Sub*);
    void (__fastcall *slot2)(Sub*);
    void (__fastcall *slot3)(Sub*);
    void (__fastcall *slot4)(Sub*);
    void (__fastcall *slot5)(Sub*);
    void (__fastcall *slot6)(Sub*);
    void (__fastcall *slot7)(Sub*);   // +0x1c
};

struct Sub {
    SubVtbl* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void* f0;      // +0x00
    Sub*  f4;      // +0x04
};

// helper @ 0xf35b30, __fastcall(this) -> bool
extern "C" bool __fastcall CQ_SunnyvaleMasterScript_helper(CQ_SunnyvaleMasterScript* self);

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        Sub* p = self->f4;
        p->vtbl->slot7(p);
    } while (!CQ_SunnyvaleMasterScript_helper(self));
}
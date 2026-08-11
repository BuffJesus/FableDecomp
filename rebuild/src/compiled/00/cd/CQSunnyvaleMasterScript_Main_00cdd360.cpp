// CQ_SunnyvaleMasterScript::Main @ 00cdd360

struct IVtblObj;
struct IVtbl {
    void (__fastcall *slot0)(IVtblObj*);
    void (__fastcall *slot1)(IVtblObj*);
    void (__fastcall *slot2)(IVtblObj*);
    void (__fastcall *slot3)(IVtblObj*);
    void (__fastcall *slot4)(IVtblObj*);
    void (__fastcall *slot5)(IVtblObj*);
    void (__fastcall *slot6)(IVtblObj*);
    void (__fastcall *slot7)(IVtblObj*);   // +0x1c
};
struct IVtblObj { IVtbl* vtbl; };

struct CQ_SunnyvaleMasterScript {
    char pad[0x40];
    IVtblObj* f40;   // +0x40
};

extern "C" char __fastcall Q_Sunny_cb7940(CQ_SunnyvaleMasterScript* self);

void __fastcall CQ_SunnyvaleMasterScript_Main(CQ_SunnyvaleMasterScript* self);

void __fastcall CQ_SunnyvaleMasterScript_Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        IVtblObj* o = self->f40;
        o->vtbl->slot7(o);
    } while (Q_Sunny_cb7940(self) == 0);
}
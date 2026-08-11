// CQ_SunnyvaleMasterScript::Main @ 00e191a0

struct IThing;
struct IThingVtbl {
    void* slot00;
    void* slot04;
    void* slot08;
    void* slot0c;
    void* slot10;
    void* slot14;
    void* slot18;
    char (__fastcall *slot1c)(IThing* self); // [eax+0x1c]
};
struct IThing {
    IThingVtbl* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void* field0;
    IThing* field4;
};

// helper at 0xf35b30, thiscall (this in ecx), returns bool in al
extern "C" char __fastcall SunnyvaleMasterHelper(CQ_SunnyvaleMasterScript* self);

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        self->field4->vtbl->slot1c(self->field4);
    } while (SunnyvaleMasterHelper(self) == 0);
}
// CQ_SunnyvaleMasterScript::Main @ 0x00dc84a0
// do { this->f4->vfn7(); } while(!helper(this));

struct Sub;
struct CQ_SunnyvaleMasterScript;

struct SubVtbl {
    void* slot0;
    void* slot1;
    void* slot2;
    void* slot3;
    void* slot4;
    void* slot5;
    void* slot6;
    void (__fastcall *slot7)(Sub*);   // [eax+0x1c]
};

struct Sub {
    SubVtbl* vtbl;
};

struct CQ_SunnyvaleMasterScript {
    void* f0;
    Sub*  f4;
};

// helper @ 0xf35b30 : bool __fastcall(CQ_SunnyvaleMasterScript*)
extern "C" bool __fastcall Q_Sunnyvale_helper_f35b30(CQ_SunnyvaleMasterScript* self);

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        self->f4->vtbl->slot7(self->f4);
    } while (!Q_Sunnyvale_helper_f35b30(self));
}
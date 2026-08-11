// CQ_SunnyvaleMasterScript::Main @ 00e192a0
struct IVObj {
    struct VT { void (__fastcall *slot0)(IVObj*); void (__fastcall *slot1)(IVObj*);
        void (__fastcall *slot2)(IVObj*); void (__fastcall *slot3)(IVObj*);
        void (__fastcall *slot4)(IVObj*); void (__fastcall *slot5)(IVObj*);
        void (__fastcall *slot6)(IVObj*); void (__fastcall *slot7_1c)(IVObj*); };
    VT* vt;
};

struct CQ_SunnyvaleMasterScript {
    void* f0;
    IVObj* f4;
};

extern "C" bool __fastcall SunnyvaleHelper_f35b30(CQ_SunnyvaleMasterScript* self);

void __fastcall CQ_SunnyvaleMasterScript_Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        self->f4->vt->slot7_1c(self->f4);
    } while (!SunnyvaleHelper_f35b30(self));
}
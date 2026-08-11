// CQ_SunnyvaleMasterScript::Main @ 00d91540
// do { this->f4->slot7(); } while (!helper(this));

struct IStep {
    struct VT { void (__fastcall *slot0)(IStep*); void (__fastcall *slot1)(IStep*);
                void (__fastcall *slot2)(IStep*); void (__fastcall *slot3)(IStep*);
                void (__fastcall *slot4)(IStep*); void (__fastcall *slot5)(IStep*);
                void (__fastcall *slot6)(IStep*); void (__fastcall *slot7)(IStep*); };
    VT* vt;
};

struct CQ_SunnyvaleMasterScript {
    void* f0;
    IStep* f4;
};

// helper at 0xf35b30
extern "C" bool __fastcall SunnyvaleMasterHelper(CQ_SunnyvaleMasterScript* self);

void __fastcall CQ_SunnyvaleMasterScript__Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        IStep* s = self->f4;
        s->vt->slot7(s);
    } while (!SunnyvaleMasterHelper(self));
}
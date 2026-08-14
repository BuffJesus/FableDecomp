struct CTCActivationTrigger;
struct SubObjA { char pad[4]; };
struct SubListF2 { void Add(SubObjA* a); };
struct CTCActivationReceptorBase {
    char       pad00[0x0c];
    SubObjA    subA;
    char       padA[0xf8 - 0x10];
    int        stateF8;
    SubListF2  listFc;
};
void __fastcall AddTrigger(CTCActivationReceptorBase* self, CTCActivationTrigger* trig)
{
    (void)trig;
    self->listFc.Add(&self->subA);
    self->stateF8 = 5;
}
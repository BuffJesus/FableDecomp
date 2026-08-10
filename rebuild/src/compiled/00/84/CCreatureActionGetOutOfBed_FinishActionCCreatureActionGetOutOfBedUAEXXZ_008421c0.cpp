#pragma optimize("y",on)

struct CCreatureVtbl {
    void (__fastcall *slot0)(void*);
    void (__fastcall *slot4)(void*);
    void (__fastcall *slot8)(void*);
    void (__fastcall *slot12)(void*);
};

struct CCreatureAction_GetOutOfBed {
    CCreatureVtbl* vtbl;
    char pad[0x62 - 4];
    bool interrupted;
};

extern void __fastcall CCreatureAction_Base_FinishAction(CCreatureAction_GetOutOfBed* self);

void __fastcall FinishAction(CCreatureAction_GetOutOfBed* self)
{
    if (!self->interrupted)
        self->vtbl->slot12(self);
    CCreatureAction_Base_FinishAction(self);
}
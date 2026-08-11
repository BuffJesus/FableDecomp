// CCreatureAction_ForcePushOutOf::FinishAction  @ 0x008b93b0
// void __fastcall (this)

struct CCreatureAction_ForcePushOutOf;

// virtual table: slot 3 (offset 0xc) = void method(this)
struct CCreatureAction_ForcePushOutOf_vtbl {
    void (__fastcall *slot0)(CCreatureAction_ForcePushOutOf*);
    void (__fastcall *slot1)(CCreatureAction_ForcePushOutOf*);
    void (__fastcall *slot2)(CCreatureAction_ForcePushOutOf*);
    void (__fastcall *slot3)(CCreatureAction_ForcePushOutOf*);
};

struct CCreatureAction_ForcePushOutOf {
    CCreatureAction_ForcePushOutOf_vtbl* vtbl;
};

// call 0x6925a0 : __fastcall(this) -> bool
extern bool __fastcall CreatureForcePush_Check(CCreatureAction_ForcePushOutOf* self);
// jmp 0x859a20 : base CCreatureAction::FinishAction(this) -> void
extern void __fastcall CreatureAction_FinishAction_Base(CCreatureAction_ForcePushOutOf* self);

void __fastcall CCreatureAction_ForcePushOutOf_FinishAction(CCreatureAction_ForcePushOutOf* self)
{
    if (!CreatureForcePush_Check(self)) {
        self->vtbl->slot3(self);
    }
    CreatureAction_FinishAction_Base(self);
}
// CCreatureAction_UnsheatheWeapons::FinishAction  (virtual, __fastcall, void)
//
//   push esi; mov esi,ecx
//   call CheckPredicate            ; bool r (ecx=this)
//   test al,al; jne skip
//   mov eax,[esi]; mov ecx,esi; call [eax+0xC]   ; virtual slot 3
//   skip:
//   mov ecx,esi; pop esi; jmp BaseFinishAction   ; tail call

struct CCreatureAction_UnsheatheWeapons;

// direct (non-virtual) member calls -> external in retail
bool __fastcall UnsheatheWeapons_CheckPredicate(CCreatureAction_UnsheatheWeapons* self);
void __fastcall UnsheatheWeapons_BaseFinishAction(CCreatureAction_UnsheatheWeapons* self);

struct CCreatureAction_UnsheatheWeapons
{
    virtual void v0();              // slot 0 (0x0)
    virtual void v1();              // slot 1 (0x4)
    virtual void v2();              // slot 2 (0x8)
    virtual void OnFinishHook();    // slot 3 (0xC)  <- the call [eax+0xC]
    virtual void FinishAction();    // this function (some later slot)
};

void CCreatureAction_UnsheatheWeapons::FinishAction()
{
    if (!UnsheatheWeapons_CheckPredicate(this))
        this->OnFinishHook();          // virtual slot 3
    UnsheatheWeapons_BaseFinishAction(this);   // tail call
}
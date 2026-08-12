// CCreatureAction_CastStrengthMultiStrike::FrameUpdate  0x008e6390
// push esi; mov esi,ecx; call base; eax=[esi]; call [eax+0x6c] (bool);
// if(al) { edx=[esi]; jmp [edx+0x10] }  else ret

struct CCreatureAction_CastStrengthMultiStrike;

// external non-virtual base call (direct call 0x859d80)
void __fastcall CCreatureAction_Base_FrameUpdate(CCreatureAction_CastStrengthMultiStrike* self);

struct CCreatureAction_CastStrengthMultiStrike
{
    // vtable laid out so slot 4 (0x10) = DoStrike (void),
    // slot 27 (0x6c) = ShouldStrike (bool)
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void DoStrike();          // slot 4  -> [vtbl+0x10]
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual bool ShouldStrike();      // slot 27 -> [vtbl+0x6c]
    virtual void FrameUpdate();       // this function
};

void CCreatureAction_CastStrengthMultiStrike::FrameUpdate()
{
    CCreatureAction_Base_FrameUpdate(this);
    if (this->ShouldStrike())
        this->DoStrike();
}
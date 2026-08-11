// ?FinishAction@CCreatureAction_SummonerLightningOrbInto@@UAEXXZ  0x008cabe0
struct CCreatureAction_Base
{
    virtual void vf0();
    virtual void vf1();
    virtual void vf2();
    virtual void OnFinishHook();   // slot 3 -> call [eax+0xc]
    void BaseFinishAction();       // __fastcall base finish (tail-call 0x859a20)
};

struct CCreatureAction_SummonerLightningOrbInto : public CCreatureAction_Base
{
    bool Check();                  // __fastcall helper 0x6925a0
    void FinishAction();
};

void CCreatureAction_SummonerLightningOrbInto::FinishAction()
{
    if (!this->Check())
        this->OnFinishHook();
    this->BaseFinishAction();
}
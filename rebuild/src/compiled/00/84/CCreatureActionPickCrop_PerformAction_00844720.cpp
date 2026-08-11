// CCreatureAction_PickCrop::PerformAction @ 0x00844720
// push esi; lea esi,[ecx+0xa8]; call Sub::M; test eax,eax; je end;
//   mov ecx,esi; pop esi; jmp Sub::M  (tail call)  else pop esi; ret

struct Sub {
    int M();   // __fastcall, returns testable value; body external
};

struct CCreatureAction_PickCrop {
    char pad[0xa8];
    Sub sub;   // at +0xa8
    void PerformAction();
};

void CCreatureAction_PickCrop::PerformAction()
{
    if (this->sub.M())
        this->sub.M();
}
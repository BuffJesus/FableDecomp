
struct Sub {
    int  SetActive(int on);   // 0x4c7590  (thiscall)
    void Refresh();           // 0x4c7c40  (thiscall)
};
struct CTCElectrocutedEffect {
    void* vt;
    Sub*  sub;   // +4
};
void __fastcall OnInitialActivate(CTCElectrocutedEffect* self)
{
    self->sub->SetActive(1);
    self->sub->Refresh();
}
// Retail 0x007fd3d0  CAIStateGroup_BuyFromSeller::BoughtSomething
// __fastcall void(this, float amount)
//   fild [0x1375550]        ; int global
//   fmul [esp+arg]          ; * float amount
//   call _ftol              ; (long) truncate
//   mov [this+0x24], eax

extern int g_BuyFromSeller_Scale; // resides at 0x01375550 in retail

struct CAIStateGroup_BuyFromSeller
{
    char pad[0x24];
    int  field_24;

    void BoughtSomething(float amount);
};

int g_BuyFromSeller_Scale = 0;

void CAIStateGroup_BuyFromSeller::BoughtSomething(float amount)
{
    this->field_24 = (int)((float)g_BuyFromSeller_Scale * amount);
}
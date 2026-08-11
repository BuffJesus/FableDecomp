// CCreatureAction_DragonFlight::Initialise  0x008f2ae0
// push esi; mov esi,ecx; call base::Initialise; fild [esi+0x10]; fmul [g];
// call __ftol; mov [esi+0x14],eax; ret

extern float g_dragonFlightScale; // dword ptr [0x122f59c]

struct CCreatureAction_Base
{
    virtual void Initialise();
};

struct CCreatureAction_DragonFlight : public CCreatureAction_Base
{
    // vtable ptr at +0x00
    char pad4[0x10 - 4]; // bring field10 to offset 0x10
    int  field10;        // [esi+0x10]
    int  field14;        // [esi+0x14]
    virtual void Initialise();
};

// base initialise: __fastcall (this in ecx, no stack args) -> model __fastcall
void __fastcall CCreatureAction_Base_Initialise(CCreatureAction_Base* self);

void CCreatureAction_DragonFlight::Initialise()
{
    CCreatureAction_Base_Initialise(this);
    this->field14 = (int)((float)this->field10 * g_dragonFlightScale);
}
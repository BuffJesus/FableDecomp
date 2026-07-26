#pragma optimize("s",on)
// CThingPhysicalSwitchDef::Transfer
struct CSerial {
    void mA(void* p);
    void mB(void* p);
    void mC(void* p);
};

struct CThingPhysicalSwitchDef {
    void BaseTransfer(CSerial* s);
};

void __fastcall CThingPhysicalSwitchDef_Transfer(CThingPhysicalSwitchDef* self, void* edx, CSerial* s)
{
    self->BaseTransfer(s);
    char* base = (char*)self;
    s->mA(base + 0x70);
    s->mB(base + 0x74);
    s->mB(base + 0x82);
    s->mC(base + 0x90);
}
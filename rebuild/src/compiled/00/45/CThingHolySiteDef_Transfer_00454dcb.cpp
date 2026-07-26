struct CArchive {
    void TransferA(void* p);
    void TransferB(void* p);
};

struct CBase {
    char pad[0x70];
    char field70[0xE];
    char field7e[0x10];
    void BaseTransfer(CArchive* ar);
};

void __fastcall CThingHolySiteDef_Transfer(CBase* self, void* edx, CArchive* ar)
{
    self->BaseTransfer(ar);
    ar->TransferA(&self->field70);
    ar->TransferB(self->field7e);
}
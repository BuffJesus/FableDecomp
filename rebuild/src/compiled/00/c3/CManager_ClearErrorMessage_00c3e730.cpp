struct IErr {
    virtual void Release(int b) = 0;   // slot 0
    virtual void Slot1() = 0;          // slot 1
    virtual void Close() = 0;          // slot 2
};
struct CManager { char pad[0x17c]; IErr* err; };

void __fastcall CManager_ClearErrorMessage(CManager* self, int edx_dummy, int arg)
{
    IErr* e = self->err;
    e->Close();
    IErr* e2 = self->err;
    if (e2)
        e2->Release(1);
    self->err = 0;
}
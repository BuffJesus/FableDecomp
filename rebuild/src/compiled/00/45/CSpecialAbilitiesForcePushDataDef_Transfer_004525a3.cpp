struct CTransferContext {
    int Xfer(void* p);
};

struct CSpecialAbilitiesForcePushDataDef {
    unsigned char _tail[0x2c];
    void Transfer(CTransferContext* ctx);
};

void CSpecialAbilitiesForcePushDataDef::Transfer(CTransferContext* ctx)
{
    ctx->Xfer(this->_tail + 0x28);
}
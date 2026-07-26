#pragma optimize("s",on)
struct CXfer;

struct CBaseDef {
    void Transfer(CXfer* xfer);
};

struct CXfer {
    void Transfer(void* obj);
};

struct CThingSwitchDef : public CBaseDef {
    char pad[0x70];
    CXfer sub;
    void Transfer(CXfer* xfer);
};

void CThingSwitchDef::Transfer(CXfer* xfer)
{
    CBaseDef::Transfer(xfer);
    xfer->Transfer(&this->sub);
}
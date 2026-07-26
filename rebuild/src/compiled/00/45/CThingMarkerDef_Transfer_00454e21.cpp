#pragma optimize("s",on)
struct CXfer {
    void M70(void* p);
    void M80(void* p);
    void M84(void* p);
};
struct CThingMarkerDef {
    void BaseTransfer(CXfer* x);
    void Transfer(CXfer* x);
};

void CThingMarkerDef::Transfer(CXfer* x)
{
    BaseTransfer(x);
    x->M70((char*)this + 0x70);
    x->M80((char*)this + 0x80);
    x->M84((char*)this + 0x84);
}
struct A {
    virtual long convert(long v);
};

struct P {
    virtual long* getVal();
};

struct CMap {
    void* pad0;
    A* a;
};

long __fastcall GetWorldMapNumber(CMap* self, void* edx, P* p)
{
    return self->a->convert(*p->getVal());
}
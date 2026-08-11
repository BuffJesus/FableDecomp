// slot4 is at vtable offset 0x10 => it is the 5th virtual (index 4).
// Model with a class carrying 5 virtuals; the 5th takes one int by value.
struct Obj {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(int a);   // vtbl+0x10
};

void __stdcall EditFrameUpdate(Obj* p)
{
    if (p) {
        p->v4(1);
    }
}
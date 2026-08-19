// CIVCountedPointer<T>::Release — drop the held ref, then clear the slot.
// __fastcall this=ecx, no args. this+0 = ctl block; block [0]=vptr, [4]=refcount.
struct Obj {
    virtual void v0();
    virtual void v1();      // slot 1 -> `mov eax,[ecx]; call [eax+4]`, this in ecx
    long rc;
};
struct CIVCP {
    Obj* p;
    void Release();
};
void CIVCP::Release() {
    Obj* cur = this->p;
    if (cur) {
        if (--cur->rc == 0) cur->v1();
        this->p = 0;
    }
}

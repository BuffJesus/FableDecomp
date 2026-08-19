// CIVCountedPointer<T>::Adopt — release the current ref, then take a raw pointer
// WITHOUT adding a reference. __fastcall this=ecx, np=stack (ret 4).
struct Obj {
    virtual void v0();
    virtual void v1();      // slot 1 -> `mov eax,[ecx]; call [eax+4]`, this in ecx
    long rc;
};
struct CIVCP {
    Obj* p;
    void Adopt(Obj* np);
};
void CIVCP::Adopt(Obj* np) {
    Obj* cur = this->p;
    if (cur) {
        if (--cur->rc == 0) cur->v1();
        this->p = 0;
    }
    this->p = np;
}

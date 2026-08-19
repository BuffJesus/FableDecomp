#pragma optimize("s",on)
// CIVCountedPointer<T>::Assign (GetMeshEffect family, 47B x7, template 0042a6c3).
// __fastcall this=ecx, other=stack (ret 4). #pragma optimize("s") (retail size peephole).
// The `this->p = 0` lives INSIDE the release branch (retail's je skips `and [esi],0`
// when cur==0), and it forces the later `o->p` reload since o may alias this.
struct Obj {
    virtual void v0();
    virtual void v1();      // vtable slot 1 -> `mov eax,[ecx]; call [eax+4]`, this in ecx
    long rc;
};
struct CIVCP {
    Obj* p;
    void Assign(const CIVCP* o);
};
void CIVCP::Assign(const CIVCP* o) {
    Obj* cur = this->p;
    if (cur != o->p) {
        if (cur) {
            if (--cur->rc == 0) cur->v1();
            this->p = 0;
        }
        Obj* np = o->p;
        this->p = np;
        if (np) ++np->rc;
    }
}
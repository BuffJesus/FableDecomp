#pragma optimize("s",on)
// Swap in new data and destroy the old through its vtable slot 0 (flag arg 1).
// __fastcall this=ecx, newData=stack (ret 4).
struct Obj { virtual void Destroy(int flags); };
struct T { Obj* p; void SetData(Obj* newData); };
void T::SetData(Obj* newData) {
    Obj* old = this->p;
    this->p = newData;
    if (old) old->Destroy(1);
}

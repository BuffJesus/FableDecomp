#pragma optimize("s",on)
// vector<CIntelligentPointer<CThing const> >::_Destroy(first, last)
// Iterates [first,last) with element stride 0x48, invoking a slot-0 virtual
// (one stack arg = 0, scalar-deleting-destructor pattern) on each element.

struct Elem {
    virtual void vfn(int flag); // slot 0 (real __fastcall member -> ecx=this, one stack arg)
    char pad[0x48 - 4];         // element is 0x48 bytes total (vptr + pad)
};

void __fastcall _Destroy(Elem* first, Elem* last)
{
    for (Elem* p = first; p != last; p = (Elem*)((char*)p + 0x48)) {
        p->vfn(0);
    }
}
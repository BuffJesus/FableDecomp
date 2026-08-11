#pragma optimize("s",on)
// vector<CIntelligentPointer<CThing const> >::_Destroy
// Range-destroy: for each 8-byte element in [first,last), call virtual slot 0 with arg 0.
// Compiled __fastcall: ecx = first, edx = last, plus one (ignored) stack arg (ret 4).

struct CIntelligentPointer
{
    virtual void Release(int flag); // slot 0
    void* pThing;                   // +4, stride = 8 (vptr + one field)
};

void __fastcall _Destroy(CIntelligentPointer* first, CIntelligentPointer* last, void* /*alloc*/)
{
    for (; first != last; ++first)
    {
        first->Release(0);
    }
}
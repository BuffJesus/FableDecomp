// Byte-exact reconstruction of 0x009f7760
//   push ebp; mov ebp,esp; push ecx; [ebp-4]=this(ecx)
//   eax=[ebp+8]; ecx=[eax]; push ecx; ecx=this; call 0x41c780
//   eax=this; ret 4
// __fastcall member taking one pointer param; forwards param->m0 to a
// value-returning method on this; returns this. Single local (this spill).

#pragma optimize("", off)

struct Param {
    void* m0;
};

struct CPersistTraits_CReactionFrequencyTraitsArray_ {
    void* callee(void* arg);   // 0x41c780, value-returning thiscall
    CPersistTraits_CReactionFrequencyTraitsArray_* tilde_CPersistTraits_CReactionFrequencyTraitsArray_(Param* p);
};

CPersistTraits_CReactionFrequencyTraitsArray_*
CPersistTraits_CReactionFrequencyTraitsArray_::tilde_CPersistTraits_CReactionFrequencyTraitsArray_(Param* p)
{
    this->callee(p->m0);
    return this;
}
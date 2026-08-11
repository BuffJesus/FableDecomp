// operator&&<...> for NScript predicate composition (retail 0x009f7810).
//
// This is the templated operator&& that folds a CIsThingAlive leaf predicate
// onto an already-composed CPredicate_And<...> chain. Retail codegen for this
// instantiation was emitted with optimization disabled (full EBP frame, the
// return/this pointer homed to [ebp-4] and reloaded around the call):
//
//   push ebp; mov ebp,esp; push ecx        ; frame + one local
//   mov [ebp-4], ecx                        ; home this (= return object)
//   mov eax,[ebp+8]; push eax                ; push the CIsThingAlive* operand
//   mov ecx,[ebp-4]; call 0x41c780           ; And-ctor(this, operand)
//   mov eax,[ebp-4]                          ; return this
//   mov esp,ebp; pop ebp; ret 4
//
// Modelled as a __fastcall member returning its own address: ecx = the
// return/this buffer, the single stacked operand is the CIsThingAlive*, and
// the body forwards to the And-constructor at 0x41c780 then returns this.
#pragma optimize("",off)

struct CIsThingAlive
{
    int m;
};

struct CPredicate_And
{
    int payload;

    // Retail 0x41c780: the CPredicate_And And-constructor (this in ecx,
    // the CIsThingAlive* operand pushed on the stack).
    void ctor_CIsThingAlive_(CIsThingAlive *rhs);

    // The operator&& body: construct into *this from the operand, return this.
    CPredicate_And *combine_CIsThingAlive_(CIsThingAlive *rhs);
};

CPredicate_And *CPredicate_And::combine_CIsThingAlive_(CIsThingAlive *rhs)
{
    ctor_CIsThingAlive_(rhs);
    return this;
}
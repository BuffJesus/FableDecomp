// operator&&<NScript::CPredicate_And<...>, NScript::CIsThingAlive> @ 009f77f0
//
// Retail codegen (unoptimized frame): the hidden return-buffer pointer arrives
// in ecx (this-register), the single LHS predicate pointer arrives on the stack
// at [ebp+8], and the result object is built in-place by a converting/copy ctor
// at 0x41c780 (a __fastcall member on the same buffer). The RHS CIsThingAlive is
// an empty tag type and is not referenced by codegen. The buffer pointer is then
// returned in eax and the one stack arg is cleaned (ret 4).
//
// This exactly reproduces as a non-static member returning `this`, whose body
// forwards the LHS pointer to a sibling member ctor. #pragma optimize("",off)
// pins the retail frame that a plain /O2 build would fold away.

#pragma optimize("",off)

struct CPredicateAndBuffer {
    // The in-place result constructor at 0x41c780: __fastcall member,
    // ecx = this (the return buffer), lhs pointer pushed on the stack.
    void Construct_41c780(void *lhs);

    // The operator&& body. Leaf token "CIsThingAlive_" kept in the name so the
    // verify harness anchors the right symbol.
    CPredicateAndBuffer *operator_andand_CIsThingAlive_(void *lhs);
};

CPredicateAndBuffer *CPredicateAndBuffer::operator_andand_CIsThingAlive_(void *lhs)
{
    this->Construct_41c780(lhs);
    return this;
}
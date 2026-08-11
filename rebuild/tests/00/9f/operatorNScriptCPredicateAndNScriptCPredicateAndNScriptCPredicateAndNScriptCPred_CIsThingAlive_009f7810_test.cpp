#include <stdio.h>

struct CIsThingAlive
{
    int m;
};

struct CPredicate_And
{
    int payload;
    void ctor_CIsThingAlive_(CIsThingAlive *rhs);
    CPredicate_And *combine_CIsThingAlive_(CIsThingAlive *rhs);
};

// Local stand-in for retail 0x41c780: construct the And predicate in place
// from the operand. Records the operand's value so we can observe the call.
void CPredicate_And::ctor_CIsThingAlive_(CIsThingAlive *rhs)
{
    this->payload = rhs ? (rhs->m + 100) : -1;
}

CPredicate_And *CPredicate_And::combine_CIsThingAlive_(CIsThingAlive *rhs)
{
    ctor_CIsThingAlive_(rhs);
    return this;
}

int main()
{
    CIsThingAlive leaf;
    leaf.m = 7;

    CPredicate_And chain;
    chain.payload = 0;

    // operator&&: fold the leaf onto the chain; must return the same object.
    CPredicate_And *r = chain.combine_CIsThingAlive_(&leaf);

    int ok = 1;
    if (r != &chain) ok = 0;
    if (chain.payload != 107) ok = 0;

    // null-operand branch
    CPredicate_And chain2;
    chain2.payload = 0;
    CPredicate_And *r2 = chain2.combine_CIsThingAlive_(0);
    if (r2 != &chain2) ok = 0;
    if (chain2.payload != -1) ok = 0;

    if (ok)
        printf("PREDICATE_AND_OK payload=%d\n", chain.payload);
    else
        printf("PREDICATE_AND_FAIL\n");

    return ok ? 0 : 1;
}
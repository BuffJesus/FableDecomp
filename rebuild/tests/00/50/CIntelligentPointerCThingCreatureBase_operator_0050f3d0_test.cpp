#include <stdio.h>

class CThingCreatureBase;

class CIntelligentPointer_CThingCreatureBase
{
public:
    CThingCreatureBase* raw;
    CThingCreatureBase* GetRaw();
    void SetRaw(CThingCreatureBase* p);
    CIntelligentPointer_CThingCreatureBase* operator=(
        CIntelligentPointer_CThingCreatureBase* rhs);
};

// local definitions of the out-of-line helpers for standalone link
CThingCreatureBase* CIntelligentPointer_CThingCreatureBase::GetRaw()
{
    return this->raw;
}
void CIntelligentPointer_CThingCreatureBase::SetRaw(CThingCreatureBase* p)
{
    this->raw = p;
}

CIntelligentPointer_CThingCreatureBase*
CIntelligentPointer_CThingCreatureBase::operator=(
    CIntelligentPointer_CThingCreatureBase* rhs)
{
    this->SetRaw(rhs->GetRaw());
    return this;
}

int main()
{
    CIntelligentPointer_CThingCreatureBase a, b;
    a.raw = (CThingCreatureBase*)0;
    b.raw = (CThingCreatureBase*)0xDEADBEEF;

    CIntelligentPointer_CThingCreatureBase* ret = a.operator=(&b);

    int ok = 1;
    if (ret != &a) ok = 0;
    if (a.raw != (CThingCreatureBase*)0xDEADBEEF) ok = 0;

    // self-assign
    b.operator=(&b);
    if (b.raw != (CThingCreatureBase*)0xDEADBEEF) ok = 0;

    if (ok) printf("IPTR_ASSIGN_OK\n");
    else    printf("IPTR_ASSIGN_FAIL\n");
    return ok ? 0 : 1;
}
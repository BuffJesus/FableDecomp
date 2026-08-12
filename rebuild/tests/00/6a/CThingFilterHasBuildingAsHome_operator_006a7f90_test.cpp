#include <stdio.h>

struct Subobject
{
    int GetId();
};

struct CThingAICreature
{
    char pad[0x174];
};

struct CThingFilter_HasBuildingAsHome
{
    int m_expected;
    bool operator()(CThingAICreature* creature);
};

bool CThingFilter_HasBuildingAsHome::operator()(CThingAICreature* creature)
{
    Subobject* s = (Subobject*)((char*)creature + 0x170);
    return s->GetId() == m_expected;
}

// local stand-in for the external thiscall helper: read the int at this (creature+0x170)
int Subobject::GetId()
{
    return *(int*)this;
}

int main()
{
    CThingAICreature c;
    *(int*)((char*)&c + 0x170) = 42;

    CThingFilter_HasBuildingAsHome f;
    f.m_expected = 42;
    bool match = f(&c);

    CThingFilter_HasBuildingAsHome g;
    g.m_expected = 7;
    bool nomatch = g(&c);

    if (match && !nomatch)
        printf("FILTER_HASBUILDING_OK\n");
    else
        printf("FILTER_FAIL\n");
    return (match && !nomatch) ? 0 : 1;
}
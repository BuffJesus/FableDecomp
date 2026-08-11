#include <cstdio>

class CThingCreatureBase;

class CIntelligentPointer
{
public:
    CThingCreatureBase* GetTarget();
    void Assign(CThingCreatureBase* p);

    CIntelligentPointer* operator=(CIntelligentPointer* rhs)
    {
        this->Assign(rhs->GetTarget());
        return this;
    }

    CThingCreatureBase* m_ptr;
};

// Local definitions of the two engine helpers for standalone link.
static int g_assigned = 0;
CThingCreatureBase* CIntelligentPointer::GetTarget()
{
    return this->m_ptr;
}
void CIntelligentPointer::Assign(CThingCreatureBase* p)
{
    this->m_ptr = p;
    g_assigned = 1;
}

int main()
{
    CIntelligentPointer a; a.m_ptr = (CThingCreatureBase*)0x1234;
    CIntelligentPointer b; b.m_ptr = (CThingCreatureBase*)0;
    CIntelligentPointer* r = (b = &a);

    bool ok = (r == &b) && (b.m_ptr == (CThingCreatureBase*)0x1234) && (g_assigned == 1);
    if (ok) { printf("INTPTR_ASSIGN_OK\n"); return 0; }
    printf("INTPTR_ASSIGN_FAIL\n"); return 1;
}
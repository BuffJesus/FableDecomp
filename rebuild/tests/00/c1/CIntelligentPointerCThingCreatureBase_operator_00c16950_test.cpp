#include <cstdio>

class CThingCreatureBase { public: int id; };

class CIntelligentPointer
{
public:
    void* m_ptr;
    CThingCreatureBase* GetTarget();
    void SetTarget(CThingCreatureBase*);
    CIntelligentPointer* operator=(CIntelligentPointer& rhs);
};

CThingCreatureBase* CIntelligentPointer::GetTarget()
{
    return (CThingCreatureBase*)this->m_ptr;
}

void CIntelligentPointer::SetTarget(CThingCreatureBase* p)
{
    this->m_ptr = (void*)p;
}

CIntelligentPointer* CIntelligentPointer::operator=(CIntelligentPointer& rhs)
{
    this->SetTarget(rhs.GetTarget());
    return this;
}

int main()
{
    CThingCreatureBase c; c.id = 77;
    CIntelligentPointer a; a.m_ptr = 0;
    CIntelligentPointer b; b.m_ptr = &c;

    CIntelligentPointer* r = (a = b);

    bool ok = (r == &a) && (a.m_ptr == &c) && (b.m_ptr == &c);
    if (ok) printf("IPTR_ASSIGN_OK\n");
    else    printf("IPTR_ASSIGN_FAIL\n");
    return ok ? 0 : 1;
}
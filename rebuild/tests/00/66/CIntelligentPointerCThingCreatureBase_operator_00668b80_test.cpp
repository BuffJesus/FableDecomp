#include <cstdio>

class CThingCreatureBase;

class CIntelligentPointer
{
public:
    CIntelligentPointer &operator=(CIntelligentPointer &other);
    CThingCreatureBase *GetObject();
    void Assign(CThingCreatureBase *obj);
    void *m_data;
};

// Local stand-ins for the two engine thiscall methods.
static void *g_lastAssigned = 0;

CThingCreatureBase *CIntelligentPointer::GetObject()
{
    return (CThingCreatureBase *)m_data;
}

void CIntelligentPointer::Assign(CThingCreatureBase *obj)
{
    m_data = (void *)obj;
    g_lastAssigned = (void *)obj;
}

CIntelligentPointer &CIntelligentPointer::operator=(CIntelligentPointer &other)
{
    Assign(other.GetObject());
    return *this;
}

int main()
{
    CIntelligentPointer a, b;
    a.m_data = (void *)0;
    b.m_data = (void *)0x1234;

    CIntelligentPointer &r = (a = b);

    bool ok = (&r == &a) && (a.m_data == (void *)0x1234) &&
              (g_lastAssigned == (void *)0x1234);
    if (ok)
        printf("OPEQ_OK\n");
    else
        printf("OPEQ_FAIL a=%p g=%p r=%p\n", a.m_data, g_lastAssigned, (void *)&r);
    return ok ? 0 : 1;
}
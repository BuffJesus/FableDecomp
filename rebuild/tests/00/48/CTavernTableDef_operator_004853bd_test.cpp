#include <stdio.h>

class CBaseTableDef
{
public:
    int m_base;
    CBaseTableDef &operator=(const CBaseTableDef &rhs);
};

// Local definition so the test links standalone.
CBaseTableDef &CBaseTableDef::operator=(const CBaseTableDef &rhs)
{
    m_base = rhs.m_base;
    return *this;
}

class CTavernTableDef : public CBaseTableDef
{
public:
    char m_flag;
    CTavernTableDef &operator=(const CTavernTableDef &rhs);
};

CTavernTableDef &CTavernTableDef::operator=(const CTavernTableDef &rhs)
{
    CBaseTableDef::operator=(rhs);
    m_flag = rhs.m_flag;
    return *this;
}

int main()
{
    CTavernTableDef a, b;
    a.m_base = 0;
    a.m_flag = 0;
    b.m_base = 0x11223344;
    b.m_flag = (char)0x7E;

    CTavernTableDef *ret = &(a = b);

    int ok = 1;
    if (ret != &a) ok = 0;
    if (a.m_base != 0x11223344) ok = 0;
    if (a.m_flag != (char)0x7E) ok = 0;

    printf(ok ? "TAVERN_ASSIGN_OK\n" : "TAVERN_ASSIGN_FAIL\n");
    return ok ? 0 : 1;
}
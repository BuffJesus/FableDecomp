// CTavernTableDef::operator= @ 0x004853bd
// Compiler-generated copy assignment: call base::operator=(rhs), then copy a
// char member at +4, return this.

class CBaseTableDef
{
public:
    int m_base;               // occupies [+0]
    // Declared only (defined in another TU) so the derived operator= must emit
    // a real call rather than inline the base assignment.
    CBaseTableDef &operator=(const CBaseTableDef &rhs);
};

class CTavernTableDef : public CBaseTableDef
{
public:
    char m_flag;              // occupies [+4]
    CTavernTableDef &operator=(const CTavernTableDef &rhs);
};

CTavernTableDef &CTavernTableDef::operator=(const CTavernTableDef &rhs)
{
    CBaseTableDef::operator=(rhs);
    m_flag = rhs.m_flag;
    return *this;
}
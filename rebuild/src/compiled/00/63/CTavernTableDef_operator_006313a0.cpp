// CTavernTableDef::operator= @ 005f9bb2
// push esi/edi; edi=src; base::operator=(this,src); this[+0xd]=src[+0xd]; return this
struct BaseDef {
    char pad[0xd];       // 0x00..0x0c
    // base operator= is a real out-of-line __fastcall; forward-declared, not inlined here
    BaseDef* AssignBase(const BaseDef* rhs);
};

struct CTavernTableDef : public BaseDef {
    char m_flag;         // +0x0d
    CTavernTableDef& operator=(const CTavernTableDef& rhs);
};

CTavernTableDef& CTavernTableDef::operator=(const CTavernTableDef& rhs)
{
    this->AssignBase(&rhs);
    this->m_flag = rhs.m_flag;
    return *this;
}
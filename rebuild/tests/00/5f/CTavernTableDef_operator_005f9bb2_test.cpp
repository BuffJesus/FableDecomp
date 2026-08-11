#include <stdio.h>
#include <string.h>

struct BaseDef {
    char pad[0xd];
    BaseDef* AssignBase(const BaseDef* rhs);
};

struct CTavernTableDef : public BaseDef {
    char m_flag;
    CTavernTableDef& operator=(const CTavernTableDef& rhs);
};

// out-of-line base operator= : copy the 0xd base bytes, return this
BaseDef* BaseDef::AssignBase(const BaseDef* rhs)
{
    memcpy(this->pad, rhs->pad, 0xd);
    return this;
}

CTavernTableDef& CTavernTableDef::operator=(const CTavernTableDef& rhs)
{
    this->AssignBase(&rhs);
    this->m_flag = rhs.m_flag;
    return *this;
}

int main()
{
    CTavernTableDef a, b;
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    for (int i = 0; i < 0xd; ++i) b.pad[i] = (char)(i + 1);
    b.m_flag = (char)0x7a;

    CTavernTableDef* r = &(a = b);

    bool ok = (r == &a);
    for (int i = 0; i < 0xd; ++i) if (a.pad[i] != (char)(i + 1)) ok = false;
    if (a.m_flag != (char)0x7a) ok = false;

    printf("m_flag=%02x r_is_this=%d\n", (unsigned char)a.m_flag, r == &a ? 1 : 0);
    if (ok) { printf("TAVERN_ASSIGN_OK\n"); return 0; }
    printf("TAVERN_ASSIGN_FAIL\n");
    return 1;
}
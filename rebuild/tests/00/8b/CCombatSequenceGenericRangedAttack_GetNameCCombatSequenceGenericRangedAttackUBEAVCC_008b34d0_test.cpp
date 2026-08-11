#include <cstdio>
#include <cstring>

// Local standalone model mirroring s.cpp (independent behaviour test).

struct CCharString {
    char* m_buf;
    CCharString(const char* s, int len) {
        int n = (len < 0) ? (int)strlen(s) : len;
        m_buf = new char[n + 1];
        memcpy(m_buf, s, n);
        m_buf[n] = 0;
    }
};

static const char kName[] = "GenericRangedAttack";

struct CCombatSequence_GenericRangedAttack {
    void* vtbl;
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_GenericRangedAttack::GetName() const
{
    return CCharString(kName, -1);
}

int main() {
    CCombatSequence_GenericRangedAttack obj;
    obj.vtbl = 0;
    CCharString s = obj.GetName();
    if (strcmp(s.m_buf, "GenericRangedAttack") == 0) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_BAD\n");
    return 1;
}
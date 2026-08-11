#include <stdio.h>

class CCharString {
public:
    CCharString(const char* s, int n);
    const char* s;
    int n;
};

// Local definition of the ctor for standalone link.
CCharString::CCharString(const char* str, int num) {
    s = str;
    n = num;
}

class CCombatSequence_BanditAttackMiddle {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditAttackMiddle::GetName() const
{
    return CCharString((const char*)0x1278790, -1);
}

int main() {
    CCombatSequence_BanditAttackMiddle obj;
    CCharString r = obj.GetName();
    if (r.s == (const char*)0x1278790 && r.n == -1) {
        printf("GETNAME_PASS\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}
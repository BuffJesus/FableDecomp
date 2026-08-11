#include <stdio.h>

class CCharString {
public:
    CCharString(const char* s, int n) { n_ = n; s_ = s; }
    int n_;
    const char* s_;
    void* pad;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123a154, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    if (r.n_ == -1 && r.s_ == (const char*)0x123a154) {
        printf("0072a710_TEST PASS\n");
    } else {
        printf("FAIL n=%d s=%p\n", r.n_, r.s_);
    }
    return 0;
}
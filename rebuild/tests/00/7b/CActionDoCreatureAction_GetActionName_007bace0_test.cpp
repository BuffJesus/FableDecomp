#include <cstdio>

struct CCharString {
    const char* p;
    int n;
    CCharString(const char* s, int len) : p(s), n(len) {}
};

struct CActionDoCreatureAction {
    char pad;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0123caf0, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    if (r.p == (const char*)0x0123caf0 && r.n == -1) {
        std::printf("007bace0_TEST PASS\n");
    } else {
        std::printf("FAIL p=%p n=%d\n", (void*)r.p, r.n);
    }
    return 0;
}
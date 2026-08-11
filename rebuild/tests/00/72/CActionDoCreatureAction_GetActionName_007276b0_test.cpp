#include <cstdio>

struct StringRep;

struct CCharString
{
    StringRep* rep;
    CCharString(const char* s, int n);
};

#pragma auto_inline(off)
CCharString::CCharString(const char* s, int n)
{
    rep = (StringRep*)(s ? (void*)(n) : 0);
}
#pragma auto_inline(on)

struct CActionDoCreatureAction
{
    void* vtbl;
    CCharString GetActionName() const;
};

CCharString CActionDoCreatureAction::GetActionName() const
{
    return CCharString("ACTION_DO_CREATURE_ACTION", -1);
}

int main()
{
    CActionDoCreatureAction obj;
    obj.vtbl = 0;
    CCharString s = obj.GetActionName();
    if (s.rep == (StringRep*)(-1))
        printf("007276b0_TEST PASS\n");
    else
        printf("FAIL rep=%p\n", (void*)s.rep);
    return 0;
}
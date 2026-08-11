#include <stdio.h>
#include <string.h>

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

const char kActionNameLiteral[] = "DoCreatureAction";

// Model ctor: len==-1 means strlen; store pointer for verification.
CCharString::CCharString(const char* s, int len)
{
    p = (char*)s;
    (void)len;
}

struct CActionDoCreatureAction {
    char dummy;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kActionNameLiteral, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    obj.dummy = 7;
    CCharString r = obj.GetActionName();
    if (r.p == kActionNameLiteral && strcmp(r.p, "DoCreatureAction") == 0) {
        printf("00771100_TEST PASS\n");
    } else {
        printf("00771100_TEST FAIL\n");
    }
    return 0;
}
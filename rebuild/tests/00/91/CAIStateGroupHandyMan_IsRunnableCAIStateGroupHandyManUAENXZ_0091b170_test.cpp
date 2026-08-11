#include <cstdio>

struct CAIStateGroup_HandyMan {
    int hval;
    int tval;
    bool IsRunnable();
};

bool __fastcall helper_91a6f0(CAIStateGroup_HandyMan* self) { return self->hval != 0; }
bool __fastcall tail_91b010(CAIStateGroup_HandyMan* self) { return self->tval != 0; }

bool CAIStateGroup_HandyMan::IsRunnable()
{
    if (helper_91a6f0(this))
        return tail_91b010(this);
    return false;
}

int main()
{
    CAIStateGroup_HandyMan a; a.hval=0; a.tval=1;
    CAIStateGroup_HandyMan b; b.hval=1; b.tval=0;
    CAIStateGroup_HandyMan c; c.hval=1; c.tval=1;
    bool ok = (a.IsRunnable()==false) && (b.IsRunnable()==false) && (c.IsRunnable()==true);
    if (ok) printf("0091b170_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}
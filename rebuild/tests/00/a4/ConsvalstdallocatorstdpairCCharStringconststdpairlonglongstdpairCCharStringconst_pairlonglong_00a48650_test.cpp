#include <stdio.h>
#include <string.h>

struct CCharString {
    char* p;
    void CopyFrom(CCharString* src) { this->p = src->p; }
};

struct PairLL { long a; long b; };
struct Pair { CCharString first; PairLL second; };

void __fastcall Cons_val(Pair* self, Pair* val)
{
    if (self) {
        self->first.CopyFrom(&val->first);
        self->second.a = val->second.a;
        self->second.b = val->second.b;
    }
}

int main()
{
    char buf[] = "hello";
    Pair src; src.first.p = buf; src.second.a = 111; src.second.b = 222;
    Pair dst; dst.first.p = 0; dst.second.a = 0; dst.second.b = 0;
    Cons_val(&dst, &src);
    // null self must be a no-op
    Cons_val(0, &src);
    if (dst.first.p == buf && dst.second.a == 111 && dst.second.b == 222)
        printf("CONSVAL_LL_OK\n");
    else
        printf("FAIL\n");
    return 0;
}
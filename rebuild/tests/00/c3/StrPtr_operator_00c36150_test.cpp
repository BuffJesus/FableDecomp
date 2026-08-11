#include <stdio.h>

struct StrPtr {
    char* p;
};

typedef int (__stdcall *CmpFn)(char*, char*);
extern CmpFn g_cmp;

int __fastcall StrPtr_operator_ne(StrPtr* self, int edx, char* rhs);

static char* g_seenA;
static char* g_seenB;
static int g_ret;

static int __stdcall myCmp(char* a, char* b)
{
    g_seenA = a;
    g_seenB = b;
    return g_ret;
}

int main()
{
    g_cmp = myCmp;

    char lhs[] = "hello";
    char rhs[] = "world";
    StrPtr sp;
    sp.p = lhs;

    // Case 1: cmp returns 0x102 -> operator!= yields 1 (true)
    g_ret = 0x102;
    int r1 = StrPtr_operator_ne(&sp, 0, rhs);

    // Case 2: cmp returns 0 -> yields 0 (false)
    g_ret = 0;
    int r2 = StrPtr_operator_ne(&sp, 0, rhs);

    // Case 3: cmp returns some other value -> yields 0
    g_ret = 5;
    int r3 = StrPtr_operator_ne(&sp, 0, rhs);

    // verify args forwarded in correct order: *this then rhs
    int argsok = (g_seenA == lhs) && (g_seenB == rhs);

    if (r1 == 1 && r2 == 0 && r3 == 0 && argsok) {
        printf("STRPTR_NE_OK\n");
        return 0;
    }
    printf("STRPTR_NE_FAIL r1=%d r2=%d r3=%d argsok=%d\n", r1, r2, r3, argsok);
    return 1;
}
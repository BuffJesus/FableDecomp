#include <cstdio>

typedef int ENavigatorType;

void __fastcall fill_ENavigatorType(ENavigatorType* _First, ENavigatorType* _Last, const ENavigatorType* _Val)
{
    for (; _First != _Last; ++_First)
        *_First = *_Val;
}

int main()
{
    ENavigatorType buf[5] = {0,0,0,0,0};
    ENavigatorType v = 7;
    fill_ENavigatorType(buf, buf + 5, &v);
    int ok = 1;
    for (int i = 0; i < 5; ++i) if (buf[i] != 7) ok = 0;
    ENavigatorType v2 = 3;
    fill_ENavigatorType(buf + 1, buf + 1, &v2);
    if (buf[1] != 7) ok = 0;
    if (ok) { std::printf("_global_0040ea50_TEST PASS\n"); return 0; }
    std::printf("_global_0040ea50_TEST FAIL\n"); return 1;
}
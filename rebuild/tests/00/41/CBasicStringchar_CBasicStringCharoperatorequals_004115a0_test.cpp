#include "fable_basic_string.h"
#include <stdio.h>

int main()
{
    char buf1[] = "hello";
    CBasicString<char> s1;
    s1.PData = buf1;

    bool r1 = (s1 == "hello");
    bool r2 = (s1 == "world");
    bool r3 = (s1 == "hell");
    bool r4 = (s1 == "hello!");

    char emptybuf[] = "";
    CBasicString<char> s2;
    s2.PData = emptybuf;
    bool r5 = (s2 == "");
    bool r6 = (s2 == "x");

    if (r1 == true && r2 == false && r3 == false && r4 == false && r5 == true && r6 == false)
    {
        printf("PASS_OP_EQ_004115A0\n");
    }
    else
    {
        printf("FAIL %d %d %d %d %d %d\n", r1, r2, r3, r4, r5, r6);
    }

    return 0;
}
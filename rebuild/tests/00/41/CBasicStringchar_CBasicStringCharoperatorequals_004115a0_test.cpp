#include <stdio.h>

struct CBasicStringChar
{
    char* m_str;

    bool operator==(const char* other) const
    {
        const char* p = other;
        const char* base = m_str - (int)p;

        int result;

        for (;;)
        {
            char a = base[(int)p];

            if (a == 0 && *p == 0)
            {
                result = 0;
                break;
            }

            char b = *p;

            if (a < b)
            {
                result = -1;
                break;
            }
            if (a > b)
            {
                result = 1;
                break;
            }

            ++p;
        }

        return result == 0;
    }
};

int main()
{
    char buf1[] = "hello";
    CBasicStringChar s1;
    s1.m_str = buf1;

    bool r1 = (s1 == "hello");
    bool r2 = (s1 == "world");
    bool r3 = (s1 == "hell");
    bool r4 = (s1 == "hello!");

    char emptybuf[] = "";
    CBasicStringChar s2;
    s2.m_str = emptybuf;
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
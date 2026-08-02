#include <cstdio>

class CStringParser
{
public:
    bool __fastcall CalcInQuotes(void* edx_unused, bool bSeed, char* p, char* pEnd);
};

bool __fastcall CStringParser::CalcInQuotes(void* edx_unused, bool bSeed, char* p, char* pEnd)
{
    bool bInQuotes = bSeed;
    while (p != pEnd)
    {
        char c = *p;
        if (c == '"' || c == '\'')
        {
            bInQuotes = !bInQuotes;
        }
        ++p;
    }
    return bInQuotes;
}

int main()
{
    CStringParser parser;

    char s1[] = "abc\"def";
    bool r1 = parser.CalcInQuotes(0, false, s1, s1 + 7);
    if (r1 != true) { printf("FAIL1 %d\n", (int)r1); return 1; }

    char s2[] = "no quotes here";
    bool r2 = parser.CalcInQuotes(0, false, s2, s2 + 15);
    if (r2 != false) { printf("FAIL2 %d\n", (int)r2); return 1; }

    char s3[] = "'a'b'c";
    bool r3 = parser.CalcInQuotes(0, true, s3, s3 + 6);
    if (r3 != false) { printf("FAIL3 %d\n", (int)r3); return 1; }

    char s4[] = "empty";
    bool r4 = parser.CalcInQuotes(0, true, s4, s4);
    if (r4 != true) { printf("FAIL4 %d\n", (int)r4); return 1; }

    char s5[] = "mix\"'\"end";
    bool r5 = parser.CalcInQuotes(0, false, s5, s5 + 9);
    if (r5 != true) { printf("FAIL5 %d\n", (int)r5); return 1; }

    printf("ALL_PASS_CALCINQUOTES\n");
    return 0;
}
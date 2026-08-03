#include <stdio.h>

class CCharString {
public:
    char *m_data;
    CCharString();
};

long g_charStringCount = 0;

CCharString::CCharString()
{
    m_data = 0;
    ++g_charStringCount;
}

int main()
{
    g_charStringCount = 5;
    CCharString s;
    if (s.m_data == 0 && g_charStringCount == 6) {
        printf("PASS_CTOR_OK\n");
    } else {
        printf("FAIL data=%p count=%ld\n", (void*)s.m_data, g_charStringCount);
    }
    return 0;
}
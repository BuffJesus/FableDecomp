#include <stdio.h>

enum ENavigatorType { NAV_A = 0, NAV_B = 1, NAV_C = 7 };

void __fastcall fill_ENavigatorType(ENavigatorType *first, ENavigatorType *last, ENavigatorType *value)
{
    for (; first != last; ++first)
        *first = *value;
}

int main()
{
    ENavigatorType buf[5];
    for (int i = 0; i < 5; ++i) buf[i] = NAV_A;
    ENavigatorType v = NAV_C;

    // Fill middle three: indices 1..3
    fill_ENavigatorType(&buf[1], &buf[4], &v);

    int ok = (buf[0] == NAV_A) && (buf[1] == NAV_C) && (buf[2] == NAV_C) &&
             (buf[3] == NAV_C) && (buf[4] == NAV_A);

    // Empty range (first == last) does nothing
    ENavigatorType w = NAV_B;
    fill_ENavigatorType(&buf[0], &buf[0], &w);
    ok = ok && (buf[0] == NAV_A);

    if (ok) printf("FILL_OK\n");
    else printf("FILL_BAD\n");
    return 0;
}
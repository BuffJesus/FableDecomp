// Behaviour of CBasicString<char>::operator< 0x00429950: true exactly when the
// text sorts before the other's under a signed byte-wise compare (a proper
// prefix sorts first; equal text is not less).
#include "fable_basic_string.h"
#include <stdio.h>

static CBasicString<char> Make(const char* text)
{
    CBasicString<char> s;
    s.PData = const_cast<char*>(text);
    return s;
}

static int Check(const char* a, const char* b, bool want)
{
    CBasicString<char> x = Make(a), y = Make(b);
    if ((x < y) != want)
    {
        printf("\"%s\" < \"%s\": want %d\n", a, b, (int)want);
        return 1;
    }
    return 0;
}

int main()
{
    if (Check("abc", "abd", true) || Check("abd", "abc", false) || Check("abc", "abc", false) ||
        Check("ab", "abc", true) || Check("abc", "ab", false) || Check("", "", false) ||
        Check("", "a", true) || Check("a", "", false) ||
        Check("\x80", "a", true) || Check("a", "\x80", false) || Check("Z", "a", true))
        return 1;
    puts("FABLETLC_CBASICSTRING_CHAR_LESS_00429950_PASS");
    return 0;
}

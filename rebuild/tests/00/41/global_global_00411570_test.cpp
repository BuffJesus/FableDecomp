// Behaviour of CBasicString<char>::Compare 0x00411570: signed byte-wise
// strcmp returning exactly -1, 0 or 1 (a proper prefix sorts first).
#define FABLE_BASIC_STRING_DECLARATIONS_ONLY
#include "fable_basic_string.h"
#include <stdio.h>

struct Probe : CBasicString<char>
{
    static long Call(const char* a, const char* b) { return Compare(a, b); }
};

static int Check(const char* a, const char* b, long want)
{
    long got = Probe::Call(a, b);
    if (got != want)
    {
        printf("Compare(\"%s\", \"%s\") = %ld want %ld\n", a, b, got, want);
        return 1;
    }
    return 0;
}

int main()
{
    if (Check("abc", "abc", 0) || Check("abc", "abd", -1) || Check("abd", "abc", 1) ||
        Check("ab", "abc", -1) || Check("abc", "ab", 1) || Check("", "", 0) ||
        Check("\x80", "a", -1) || Check("a", "\x80", 1) || Check("Z", "a", -1))
        return 1;
    puts("FABLETLC_CBASICSTRING_CHAR_COMPARE_00411570_PASS");
    return 0;
}

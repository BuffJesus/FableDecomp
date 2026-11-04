#include "trim_string_fixture.h"
int main()
{
    { CCharString value(""); value.RemoveWhiteSpaceEnd(); requireTrim(!substringCalls && !value.GetLength()); }
    { CCharString value("  abc \t\r\n"); value.RemoveWhiteSpaceEnd();
      requireTrim(!strcmp(value.GetConstCharPtr(), "  abc") && substringCalls == 1); }
    { CCharString value("  abc"); value.RemoveWhiteSpaceEnd();
      requireTrim(!strcmp(value.GetConstCharPtr(), "  abc") && substringCalls == 1); }
    { CCharString value(" \t\r\n"); value.RemoveWhiteSpaceEnd();
      requireTrim(!strcmp(value.GetConstCharPtr(), " \t\r\n") && substringCalls == 1); }
    { CCharString value("abc  "); CCharString shared(value); value.RemoveWhiteSpaceEnd();
      requireTrim(!strcmp(value.GetConstCharPtr(), "abc") &&
                  !strcmp(shared.GetConstCharPtr(), "abc  ") && substringCalls == 2); }
    requireTrim(storageLive == 0 && g_CCharStringInstanceCount_013BD800 == 0);
    puts("CHARSTRING_TRIM_END_PASS");
    return 0;
}

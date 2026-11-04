#include "trim_string_fixture.h"
int main()
{
    { CCharString value(""); value.RemoveWhiteSpaceBegin(); requireTrim(!substringCalls && !value.GetLength()); }
    { CCharString value("  \t\r\nabc  "); value.RemoveWhiteSpaceBegin();
      requireTrim(!strcmp(value.GetConstCharPtr(), "abc  ") && substringCalls == 1); }
    { CCharString value("abc  "); value.RemoveWhiteSpaceBegin();
      requireTrim(!strcmp(value.GetConstCharPtr(), "abc  ") && substringCalls == 1); }
    { CCharString value(" \t\r\n"); value.RemoveWhiteSpaceBegin();
      requireTrim(!strcmp(value.GetConstCharPtr(), " \t\r\n") && substringCalls == 1); }
    { CCharString value("  abc"); CCharString shared(value); value.RemoveWhiteSpaceBegin();
      requireTrim(!strcmp(value.GetConstCharPtr(), "abc") &&
                  !strcmp(shared.GetConstCharPtr(), "  abc") && substringCalls == 2); }
    requireTrim(storageLive == 0 && g_CCharStringInstanceCount_013BD800 == 0);
    puts("CHARSTRING_TRIM_BEGIN_PASS");
    return 0;
}

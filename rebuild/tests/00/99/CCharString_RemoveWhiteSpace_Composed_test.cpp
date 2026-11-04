#include "trim_string_fixture.h"

int main()
{
    { CCharString value(""); value.RemoveWhiteSpaceBeginEnd();
      requireTrim(value.GetLength() == 0 && substringCalls == 0); }
    { CCharString value(" \t\r\nalpha beta \t\r\n"); CCharString shared(value);
      value.RemoveWhiteSpaceBeginEnd();
      requireTrim(!strcmp(value.GetConstCharPtr(), "alpha beta") &&
                  !strcmp(shared.GetConstCharPtr(), " \t\r\nalpha beta \t\r\n") &&
                  substringCalls == 2); }
    { CCharString value(" \t\r\n"); value.RemoveWhiteSpaceBeginEnd();
      requireTrim(!strcmp(value.GetConstCharPtr(), " \t\r\n") && substringCalls == 2); }
    { CCharString value("inner text"); value.RemoveWhiteSpaceBeginEnd();
      requireTrim(!strcmp(value.GetConstCharPtr(), "inner text") && substringCalls == 2); }
    requireTrim(storageLive == 0 && g_CCharStringInstanceCount_013BD800 == 0);
    puts("CHARSTRING_TRIM_COMPOSED_PASS");
    return 0;
}

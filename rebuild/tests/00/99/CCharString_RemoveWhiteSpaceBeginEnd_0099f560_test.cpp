#include <stdio.h>
#include "fable_string.h"

static int calls;
static CCharString* receiver;
void CCharString::RemoveWhiteSpaceBegin()
{
    if (++calls != 1)
        calls = -100;
    receiver = this;
}
void CCharString::RemoveWhiteSpaceEnd()
{
    if (++calls != 2)
        calls = -100;
    receiver = this;
}
int main()
{
    unsigned long raw = 0;
    CCharString& value = *reinterpret_cast<CCharString*>(&raw);
    value.RemoveWhiteSpaceBeginEnd();
    if (calls != 2 || receiver != &value)
        return 1;
    puts("CHARSTRING_TRIM_BOTH_PASS");
    return 0;
}

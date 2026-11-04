#include "fable_console.h"
#include <stdio.h>
#include <string.h>

static const CCharString* lastLeft;
static const char* lastRight;
static int calls;

CCharString::CCharString()
{
    *reinterpret_cast<void**>(this) = reinterpret_cast<void*>(0x12345678);
}
CCharString::CCharString(const CCharString& other)
{
    *reinterpret_cast<void**>(this) = *reinterpret_cast<void* const*>(&other);
}
CCharString::~CCharString() {}
CCharString FABLE_FASTCALL operator+(const CCharString& left, const char* right)
{
    lastLeft = &left;
    lastRight = right;
    ++calls;
    return CCharString();
}

class CConsoleInputTestAccess
{
public:
    static CCharString GetFormatted(const CConsoleVariable* variable)
    {
        return variable->CConsoleVariable::GetFormattedName();
    }
};

int main()
{
    int cases = 0;
    for (unsigned long p = 0; p != 16; ++p) {
        const CConsoleVariable* variable = reinterpret_cast<const CConsoleVariable*>(0x10000 + p * 4);
        CCharString result = CConsoleInputTestAccess::GetFormatted(variable);
        if (calls != static_cast<int>(p + 1) ||
            lastLeft != reinterpret_cast<const CCharString*>(reinterpret_cast<const char*>(variable) + 4) ||
            strcmp(lastRight, " ") != 0 ||
            *reinterpret_cast<void* const*>(&result) != reinterpret_cast<void*>(0x12345678)) {
            printf("FAIL case=%lu calls=%d left=%p right=%s\n", p, calls, lastLeft, lastRight);
            return 1;
        }
        ++cases;
    }
    printf("CONSOLE_VARIABLE_FORMATTED_NAME_PASS cases=%d\n", cases);
    return 0;
}

#include "fable_console.h"
#include <stdio.h>
#include <string.h>

static long lastLength;
static int ctorCalls;
CCharString::CCharString(const char* text, long length)
{
    *reinterpret_cast<const char**>(this) = text;
    lastLength = length;
    ++ctorCalls;
}
CCharString::~CCharString() {}

class CConsoleInputTestAccess
{
public:
    static CCharString GetType(const CConsoleCommandBase* command)
    {
        return command->CConsoleCommandBase::GetTypeAsString();
    }
};

int main()
{
    int cases = 0;
    for (unsigned long p = 0; p != 16; ++p) {
        const CConsoleCommandBase* command = reinterpret_cast<const CConsoleCommandBase*>(0x10000 + p * 4);
        CCharString result = CConsoleInputTestAccess::GetType(command);
        const char* text = *reinterpret_cast<const char* const*>(&result);
        if (strcmp(text, "command") != 0 || lastLength != -1 || ctorCalls != static_cast<int>(p + 1)) {
            printf("FAIL case=%lu text=%s length=%ld calls=%d\n", p, text, lastLength, ctorCalls);
            return 1;
        }
        ++cases;
    }
    printf("CONSOLE_COMMAND_TYPE_STRING_PASS cases=%d\n", cases);
    return 0;
}

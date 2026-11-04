#include "fable_console.h"
#include <stdio.h>

static const CGenericVarVector* g_received;
static int g_calls;

CCharString::CCharString(const char*, long) : storage_(0) {}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) {}
CCharString::~CCharString() {}
CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType)
    : ParamTypes() {}
CConsoleCommandParameters::~CConsoleCommandParameters() {}
bool CConsoleCommandBase::ProcessCommandLine(const CCharString&, CConsole&) { return false; }
CCharString CConsoleCommandBase::GetFormattedName() const { return CCharString(); }
CCharString CConsoleCommandBase::GetTypeAsString() const { return CCharString(); }
CCharString CConsoleCommandBase::GetHelp() { return CCharString(); }
CCharString::CCharString() : storage_(0) {}

static void FABLE_FASTCALL callback(const CGenericVarVector& params)
{
    g_received = &params;
    ++g_calls;
}

class CConsoleInputTestAccess
{
public:
    static void Call(CConsoleCommandBase& command, const CGenericVarVector& params)
    {
        command.CallCommandFunction(params);
    }
};

int main()
{
    CConsoleClasslessCommand command(CCharString("test"), &callback, GENERIC_VAR_NULL);
    unsigned char bytes[4] = {0};
    const CGenericVarVector& params =
        *reinterpret_cast<const CGenericVarVector*>(bytes);
    CConsoleInputTestAccess::Call(command, params);
    if (g_calls != 1 || g_received != &params)
        return 1;
    CConsoleInputTestAccess::Call(command, params);
    if (g_calls != 2 || g_received != &params)
        return 2;
    puts("CLASSLESS_COMMAND_CALL_OK");
    return 0;
}

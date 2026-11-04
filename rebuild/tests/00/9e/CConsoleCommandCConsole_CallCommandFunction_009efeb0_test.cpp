#include "fable_console.h"
#include <stdio.h>

static CConsole* g_receiver;
static const CGenericVarVector* g_params;
static int g_calls;

CCharString::CCharString(const char*, long) : storage_(0) {}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) {}
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}
CConsoleCommandParameters::~CConsoleCommandParameters() {}
bool CConsoleCommandBase::ProcessCommandLine(const CCharString&, CConsole&) { return false; }
CCharString CConsoleCommandBase::GetFormattedName() const { return CCharString(); }
CCharString CConsoleCommandBase::GetTypeAsString() const { return CCharString(); }
CCharString CConsoleCommandBase::GetHelp() { return CCharString(); }

static void FABLE_FASTCALL callback(CConsole& console, const CGenericVarVector& params)
{
    g_receiver = &console;
    g_params = &params;
    ++g_calls;
}

int main()
{
    unsigned char receiver_storage[4] = {0};
    CConsole& receiver = *reinterpret_cast<CConsole*>(receiver_storage);
    unsigned char parameter_storage[4] = {0};
    const CGenericVarVector& params =
        *reinterpret_cast<const CGenericVarVector*>(parameter_storage);
    CConsoleCommand<CConsole> command(CCharString("test"), receiver, &callback);
    command.CallCommandFunction(params);
    if (g_calls != 1 || g_receiver != &receiver || g_params != &params ||
        sizeof(command) != 0x1C)
        return 1;
    command.CallCommandFunction(params);
    if (g_calls != 2 || g_receiver != &receiver || g_params != &params)
        return 2;
    puts("BOUND_CONSOLE_COMMAND_CALL_OK");
    return 0;
}

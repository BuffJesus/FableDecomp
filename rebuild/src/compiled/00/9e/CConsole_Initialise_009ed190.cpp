#define FABLE_CONSOLE_PARAMETER_BASE_INLINE __declspec(noinline)
#include <new>
#include "fable_console.h"
#include "fable_stlport_uninitialized.h"
#include "fable_stlport_vector_impl.h"
// This unit expands the scalar-parameter constructor and vector teardown.
__forceinline CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType type)
    : ParamTypes() {
    ParamTypes.push_back(type);
}
__forceinline CConsoleCommandParameters::~CConsoleCommandParameters() {}
void CConsole::Initialise(char activateKeyChar, EInputKey activateKey, const CFontBank* font) {
    ToggleChar=activateKeyChar; ToggleKey=activateKey; PFont=font;
    RegisteredConsoleInputs.clear();
    AddInput(new CConsoleCommand<CConsole>("BindString",*this,&ConsoleAddStringBinding, CConsoleCommandParameters(GENERIC_VAR_STRING,GENERIC_VAR_STRING)));
    AddInput(new CConsoleCommand<CConsole>("RunBoundString",*this,&ConsoleRunStringBinding, CConsoleCommandParameters(GENERIC_VAR_STRING)));
    AddInput(new CConsoleCommand<CConsole>("BindKey",*this,&ConsoleAddKeyBinding, CConsoleCommandParameters(GENERIC_VAR_STRING,GENERIC_VAR_STRING)));
    AddInput(new CConsoleCommand<CConsole>("RunScript",*this,&ConsoleRunScript, GENERIC_VAR_STRING));
    AddInput(new CConsoleCommand<CConsole>("ConsoleListContaining",*this,&ConsoleConsoleListContainingString, GENERIC_VAR_STRING));
    AddInput(new CConsoleCommand<CConsole>("CommandList",*this,&ConsoleCommandList));
    AddInput(new CConsoleCommand<CConsole>("CommandListContaining",*this,&ConsoleCommandListContainingString, GENERIC_VAR_STRING));
    AddInput(new CConsoleCommand<CConsole>("VarList",*this,&ConsoleVariableList));
    AddInput(new CConsoleCommand<CConsole>("VarListContaining",*this,&ConsoleVariableListContainingString, GENERIC_VAR_STRING));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleTextColour",*this,&SetConsoleTextColour, CConsoleCommandParameters(GENERIC_VAR_ULONG,GENERIC_VAR_ULONG,GENERIC_VAR_ULONG)));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleHistoryWindowColour",*this,&SetConsoleHistoryWindowColour, CConsoleCommandParameters(GENERIC_VAR_ULONG,GENERIC_VAR_ULONG,GENERIC_VAR_ULONG)));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleClientWindowColour",*this,&SetConsoleClientWindowColour, CConsoleCommandParameters(GENERIC_VAR_ULONG,GENERIC_VAR_ULONG,GENERIC_VAR_ULONG)));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleHelpWindowColour",*this,&SetConsoleHelpWindowColour, CConsoleCommandParameters(GENERIC_VAR_ULONG,GENERIC_VAR_ULONG,GENERIC_VAR_ULONG)));
    AddInput(new CConsoleCommand<CConsole>("SetConsoleScrollBarColour",*this,&SetConsoleScrollBarColour, CConsoleCommandParameters(GENERIC_VAR_ULONG,GENERIC_VAR_ULONG,GENERIC_VAR_ULONG)));
    AddInput(new CConsoleVariable("ConsoleAlpha",ConsoleAlpha));
    CommandLineHistory.clear();
    HistoryPos=CommandLineHistory.begin();
    SendMessageToUser("Try and shut up and get on with your work.");
}

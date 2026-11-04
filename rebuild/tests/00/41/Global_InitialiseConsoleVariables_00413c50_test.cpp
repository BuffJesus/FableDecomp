// Behaviour fixture for InitialiseConsoleVariables (retail 0x413c50).
//
// The reconstruction calls the real console API; this file defines those callees as
// recorders and checks the registered inputs (name, kind, variable type and address, or
// command function and parameter type) against the retail order, the singleton's lazy
// creation, a failed allocation (null input, no name temporary), temporary lifetimes, and
// the two ini scripts.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "fable_console.h"
#include "fable_filesystem.h"
#include "fable_string.h"

void FABLE_FASTCALL InitialiseConsoleVariables();

// ---- globals the registrar references -------------------------------------------------

bool g_FableUsePhysicalDVD_013B85F2;
bool g_FableUseRetailSaveGameSystem_013B8646;
bool g_FableRunFromDVD_013B8615;
bool GUseRetailBanks;
bool g_FableUseCompiledAnimationEvents_013B860A;
bool g_FableUseCompiledGlobalThings_013B8609;
bool g_FableUseCompiledDefs_013B8617;
bool g_FableUseCompiledWorldFiles_013B8618;
bool g_FableUseCompiledSoundSymbols_013B8619;
bool g_FableAllowDataGeneration_01375459;
long g_FableInstallerBufferSize_01375494;
long g_FableNoInstallBuffers_01375498;
CEngineMemoryInit g_FableEngineMemoryInit_0137549C;
bool g_FableRunUserScript_01375444;
CCountedPointer<CConsole> g_FableConsolePClass_013CAA40;

// Distinct bodies, so the linker's identical-COMDAT folding cannot merge their addresses.
int g_lastCommand;
void FABLE_FASTCALL GConsoleSetLevel(const CGenericVarVector&) { g_lastCommand = 1; }
void FABLE_FASTCALL GConsoleSetStartingHolySite(const CGenericVarVector&) { g_lastCommand = 2; }
void FABLE_FASTCALL GConsoleEnableCodeSectionLoading(const CGenericVarVector&) { g_lastCommand = 3; }

// ---- recorders ----------------------------------------------------------------------

namespace
{
    enum { MaxEvents = 64 };

    size_t g_allocationSizes[MaxEvents];
    int g_allocationCount;
    int g_failAllocation = -1;

    CConsole* g_constructedConsole;
    int g_consoleConstructions;
    int g_resetCalls;
    CConsole* g_resetData;

    CConsole* g_initialiseThis;
    int g_initialiseCalls;
    char g_initialiseChar;
    EInputKey g_initialiseKey;
    const CFontBank* g_initialiseFont;

    CConsoleInputBase* g_inputs[MaxEvents];
    CConsole* g_inputThis[MaxEvents];
    int g_inputCount;

    const wchar_t* g_scripts[4];
    int g_scriptCount;
    bool g_pathExists;
    const wchar_t* g_pathExistsQuery;
    int g_pathExistsCalls;

    int g_charLiteralConstructions;
    int g_charCopies;
    int g_charDestructions;
    int g_wideConstructions;
    int g_wideDestructions;

    unsigned char g_presetConsole[sizeof(CConsole)];
}

void* operator new(size_t size)
{
    int index = g_allocationCount++;
    if (index < MaxEvents)
        g_allocationSizes[index] = size;
    if (index == g_failAllocation)
        return 0;
    return malloc(size);
}

void operator delete(void* data)
{
    free(data);
}

template<>
void CCountedPointer<CConsole>::Reset(CConsole* data)
{
    ++g_resetCalls;
    g_resetData = data;
    if (this == &g_FableConsolePClass_013CAA40)
        Data = data;
}

CConsole::CConsole()
{
    // Exclude the two command-line member literals from registrar temporaries.
    g_charLiteralConstructions -= 2;
    ++g_consoleConstructions;
    g_constructedConsole = this;
}

// Member dependencies of the typed console recorder.
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
namespace _STL { template<class T,class A> list<T,A>::~list(){free(_M_node);} }
CConsole::~CConsole() {
    free(KeyBindingMap._M_t._M_header._M_data);
    free(StringBindingMap._M_t._M_header._M_data);
    free(RegisteredConsoleInputs._M_t._M_header._M_data);
}
CCharString::CCharString() : storage_(0) {}

void CConsole::Initialise(char activateKeyChar, EInputKey activateKey, const CFontBank* font)
{
    ++g_initialiseCalls;
    g_initialiseThis = this;
    g_initialiseChar = activateKeyChar;
    g_initialiseKey = activateKey;
    g_initialiseFont = font;
}

void CConsole::AddInput(CConsoleInputBase* input)
{
    if (g_inputCount < MaxEvents)
    {
        g_inputThis[g_inputCount] = this;
        g_inputs[g_inputCount] = input;
    }
    ++g_inputCount;
}

void CConsole::RunScript(const CWideString& filename)
{
    if (g_scriptCount < 4)
        g_scripts[g_scriptCount] = filename;
    ++g_scriptCount;
}

bool FABLE_FASTCALL CAFile::PathExists(const CWideString& pathname)
{
    ++g_pathExistsCalls;
    g_pathExistsQuery = pathname;
    return g_pathExists;
}

// Stand-in storage uses malloc so operator new only sees the registrar's allocations.
// The strings keep the literal's address so the fixture can compare names.
CCharString::CCharString(const char* text, long length)
{
    ++g_charLiteralConstructions;
    storage_ = static_cast<CCharStringData*>(malloc(sizeof(CCharStringData)));
    storage_->text = const_cast<char*>(text);
    storage_->unknown04 = static_cast<fable_u32>(length);
    storage_->owners = 1;
}

CCharString::CCharString(const CCharString& other)
{
    ++g_charCopies;
    storage_ = other.storage_;
    ++storage_->owners;
}

CCharString::~CCharString()
{
    ++g_charDestructions;
    if (storage_ && --storage_->owners == 0)
        free(storage_);
}

CCharString::operator const char*() const
{
    return storage_->text;
}

CWideString::CWideString(const wchar_t* text)
{
    ++g_wideConstructions;
    storage_ = static_cast<CWideStringData*>(malloc(sizeof(CWideStringData)));
    storage_->text = const_cast<wchar_t*>(text);
}

CWideString::~CWideString()
{
    ++g_wideDestructions;
    free(storage_);
}

CWideString::operator const wchar_t*() const
{
    return storage_->text;
}

CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType type)
{
    ParamTypes._M_start = static_cast<EGenericVarType*>(malloc(sizeof(EGenericVarType)));
    *ParamTypes._M_start = type;
    ParamTypes._M_finish = ParamTypes._M_start + 1;
    ParamTypes._M_end_of_storage._M_data = ParamTypes._M_finish;
}

CConsoleCommandParameters::~CConsoleCommandParameters()
{
    // The typed ParamTypes member frees its own storage.
}

// Virtuals the emitted vtables reference; the registrar never calls them.
bool CConsoleVariable::ProcessCommandLine(const CCharString&, CConsole&) { return false; }
CCharString CConsoleVariable::GetFormattedName() const { return Name; }
CCharString CConsoleVariable::GetTypeAsString() const { return Name; }
CCharString CConsoleVariable::GetHelp() { return Name; }
bool CConsoleCommandBase::ProcessCommandLine(const CCharString&, CConsole&) { return false; }
CCharString CConsoleCommandBase::GetFormattedName() const { return Name; }
CCharString CConsoleCommandBase::GetTypeAsString() const { return Name; }
CCharString CConsoleCommandBase::GetHelp() { return Name; }
void CConsoleClasslessCommand::CallCommandFunction(const CGenericVarVector&) const {}

// ---- expectations (retail order and operands) ---------------------------------------

namespace
{
    // Retail layouts, read directly (the members are private to the console classes).
    struct VariableView
    {
        void* vtable;
        CCharString name;
        EGenericVarType type;
        void* target;
    };

    struct CommandView
    {
        void* vtable;
        CCharString name;
        CConsoleCommandParameters params;
        CConsoleCommandFunction function;
    };

    struct Expected
    {
        const char* name;
        bool command;
        EGenericVarType type;
        void* target;
        CConsoleCommandFunction function;
    };

    CEngineMemoryInit& Memory = g_FableEngineMemoryInit_0137549C;

    const Expected kExpected[] =
    {
        { "SetLevel", true, GENERIC_VAR_STRING, 0, &GConsoleSetLevel },
        { "SetStartingHolySite", true, GENERIC_VAR_STRING, 0, &GConsoleSetStartingHolySite },
        { "EnableCodeSectionLoading", true, GENERIC_VAR_BOOL, 0, &GConsoleEnableCodeSectionLoading },
        { "UsePhysicalDVD", false, GENERIC_VAR_BOOL, &g_FableUsePhysicalDVD_013B85F2, 0 },
        { "UseRetailSaveGameSystem", false, GENERIC_VAR_BOOL, &g_FableUseRetailSaveGameSystem_013B8646, 0 },
        { "RunFromDVD", false, GENERIC_VAR_BOOL, &g_FableRunFromDVD_013B8615, 0 },
        { "UseRetailBanks", false, GENERIC_VAR_BOOL, &GUseRetailBanks, 0 },
        { "UseCompiledAnimationEvents", false, GENERIC_VAR_BOOL, &g_FableUseCompiledAnimationEvents_013B860A, 0 },
        { "UseCompiledGlobalThings", false, GENERIC_VAR_BOOL, &g_FableUseCompiledGlobalThings_013B8609, 0 },
        { "UseCompiledDefs", false, GENERIC_VAR_BOOL, &g_FableUseCompiledDefs_013B8617, 0 },
        { "UseCompiledWorldFiles", false, GENERIC_VAR_BOOL, &g_FableUseCompiledWorldFiles_013B8618, 0 },
        { "UseCompiledSoundSymbols", false, GENERIC_VAR_BOOL, &g_FableUseCompiledSoundSymbols_013B8619, 0 },
        { "AllowDataGeneration", false, GENERIC_VAR_BOOL, &g_FableAllowDataGeneration_01375459, 0 },
        { "InstallerBufferSize", false, GENERIC_VAR_SLONG, &g_FableInstallerBufferSize_01375494, 0 },
        { "NoInstallBuffers", false, GENERIC_VAR_SLONG, &g_FableNoInstallBuffers_01375498, 0 },
        { "LandscapePoolSize", false, GENERIC_VAR_SLONG, &Memory.LandscapePoolSize, 0 },
        { "LandscapePhysicalMemoryRatio", false, GENERIC_VAR_FLOAT, &Memory.LandscapePhysicalMemoryRatio, 0 },
        { "LandscapeVirtualMemoryMinSize", false, GENERIC_VAR_SLONG, &Memory.LandscapeVirtualMemoryMinSize, 0 },
        { "MeshPoolSize", false, GENERIC_VAR_SLONG, &Memory.MeshPoolSize, 0 },
        { "MeshStatsPoolSize", false, GENERIC_VAR_SLONG, &Memory.MeshStatsPoolSize, 0 },
        { "MeshPhysicalMemoryRatio", false, GENERIC_VAR_FLOAT, &Memory.MeshPhysicalMemoryRatio, 0 },
        { "HiresTextureMemory", false, GENERIC_VAR_SLONG, &Memory.HiresTextureMemory, 0 },
        { "PhysicsMeshPoolSize", false, GENERIC_VAR_SLONG, &Memory.PhysicsMeshPoolSize, 0 },
        { "AnimationPoolSize", false, GENERIC_VAR_SLONG, &Memory.AnimationPoolSize, 0 },
        { "ClothPoolSize", false, GENERIC_VAR_SLONG, &Memory.ClothPoolSize, 0 },
    };

    const int kInputs = sizeof(kExpected) / sizeof(kExpected[0]);

    void* g_variableVtable;
    void* g_commandVtable;

    void Reset(CConsole* preset, int failAllocation, bool pathExists, bool runUserScript)
    {
        memset(g_allocationSizes, 0, sizeof(g_allocationSizes));
        g_allocationCount = 0;
        g_failAllocation = failAllocation;
        g_constructedConsole = 0;
        g_consoleConstructions = 0;
        g_resetCalls = 0;
        g_resetData = 0;
        g_initialiseThis = 0;
        g_initialiseCalls = 0;
        g_initialiseChar = 0;
        g_initialiseKey = KB_NULL;
        g_initialiseFont = reinterpret_cast<const CFontBank*>(1);
        g_inputCount = 0;
        g_scriptCount = 0;
        g_pathExists = pathExists;
        g_pathExistsQuery = 0;
        g_pathExistsCalls = 0;
        g_charLiteralConstructions = 0;
        g_charCopies = 0;
        g_charDestructions = 0;
        g_wideConstructions = 0;
        g_wideDestructions = 0;
        g_FableRunUserScript_01375444 = runUserScript;
        g_FableConsolePClass_013CAA40.Data = preset;
        g_FableConsolePClass_013CAA40.Info = 0;
    }

    // Checks the inputs; `failed` is the input index whose allocation returned null (or -1).
    int CheckInputs(CConsole* console, int failed)
    {
        if (g_inputCount != kInputs)
            return 1;
        for (int index = 0; index < kInputs; ++index)
        {
            const Expected& expected = kExpected[index];
            if (g_inputThis[index] != console)
                return 2;
            if (index == failed)
            {
                if (g_inputs[index] != 0)
                    return 3;
                continue;
            }
            if (g_inputs[index] == 0)
                return 4;
            if (expected.command)
            {
                const CommandView* view = reinterpret_cast<const CommandView*>(g_inputs[index]);
                if (view->vtable != g_commandVtable ||
                    strcmp(view->name, expected.name) != 0 ||
                    view->params.ParamTypes._M_finish - view->params.ParamTypes._M_start != 1 ||
                    *view->params.ParamTypes._M_start != expected.type ||
                    view->function != expected.function)
                    return 5;
            }
            else
            {
                const VariableView* view = reinterpret_cast<const VariableView*>(g_inputs[index]);
                if (view->vtable != g_variableVtable ||
                    strcmp(view->name, expected.name) != 0 ||
                    view->type != expected.type ||
                    view->target != expected.target)
                    return 6;
            }
        }
        return 0;
    }

    int CheckTemporaries(int built)
    {
        // One literal temporary per constructed input, copied into it and destroyed after
        // AddInput; the copy stays alive in the input.
        if (g_charLiteralConstructions != built ||
            g_charCopies != built ||
            g_charDestructions != built)
            return 1;
        if (g_wideConstructions != g_wideDestructions)
            return 2;
        return 0;
    }

    int CheckInputSizes(int first)
    {
        for (int index = 0; index < kInputs; ++index)
        {
            size_t expected = kExpected[index].command ? 0x18 : 0x10;
            if (g_allocationSizes[first + index] != expected)
                return 1;
        }
        return 0;
    }
}

int main()
{
    {
        bool flag;
        CConsoleVariable variable("probe", flag);
        CConsoleClasslessCommand command("probe", &GConsoleSetLevel, GENERIC_VAR_STRING);
        g_variableVtable = *reinterpret_cast<void**>(&variable);
        g_commandVtable = *reinterpret_cast<void**>(&command);
    }

    // 1. No console yet: the first Get() creates it (0x9C bytes) and hands it to Reset once;
    //    default_userst.ini exists, the user script is off.
    Reset(0, -1, true, false);
    InitialiseConsoleVariables();
    if (g_consoleConstructions != 1 || g_resetCalls != 1 ||
        g_allocationSizes[0] != 0x9C ||
        g_resetData != g_constructedConsole ||
        g_FableConsolePClass_013CAA40.Data != g_constructedConsole)
        return 10;
    if (g_initialiseCalls != 1 || g_initialiseThis != g_constructedConsole ||
        g_initialiseChar != 'a' || g_initialiseKey != KB_A || g_initialiseFont != 0)
        return 11;
    if (int failure = CheckInputs(g_constructedConsole, -1))
        return 20 + failure;
    if (CheckInputSizes(1) || g_allocationCount != 1 + kInputs)
        return 30;
    if (g_pathExistsCalls != 1 || wcscmp(g_pathExistsQuery, L"default_userst.ini") != 0 ||
        g_scriptCount != 1 || wcscmp(g_scripts[0], L"default_userst.ini") != 0)
        return 31;
    if (int failure = CheckTemporaries(kInputs))
        return 40 + failure;

    // 2. Existing console: nothing is created; no default script, user script on.
    CConsole* preset = reinterpret_cast<CConsole*>(g_presetConsole);
    Reset(preset, -1, false, true);
    InitialiseConsoleVariables();
    if (g_consoleConstructions != 0 || g_resetCalls != 0 || g_initialiseThis != preset)
        return 50;
    if (int failure = CheckInputs(preset, -1))
        return 60 + failure;
    if (CheckInputSizes(0) || g_allocationCount != kInputs)
        return 70;
    if (g_pathExistsCalls != 1 || g_scriptCount != 1 ||
        wcscmp(g_scripts[0], L"userst.ini") != 0)
        return 71;
    if (int failure = CheckTemporaries(kInputs))
        return 80 + failure;

    // 3. Both scripts, and every input allocation failing in turn: that input is registered
    //    as null and its name temporary is never built (nor destroyed).
    for (int failed = 0; failed < kInputs; ++failed)
    {
        Reset(preset, failed, true, true);
        InitialiseConsoleVariables();
        if (int failure = CheckInputs(preset, failed))
            return 100 + failure;
        if (int failure = CheckTemporaries(kInputs - 1))
            return 110 + failure;
        if (g_scriptCount != 2 ||
            wcscmp(g_scripts[0], L"default_userst.ini") != 0 ||
            wcscmp(g_scripts[1], L"userst.ini") != 0)
            return 120;
    }

    puts("FABLETLC_INITIALISE_CONSOLE_VARIABLES PASS");
    return 0;
}

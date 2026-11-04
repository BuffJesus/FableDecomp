#pragma once

// The developer console's registration API (CConsole and its inputs), with retail layouts.
// Evidence: retail RTTI/vtables at 0x122e5b0 (CConsoleInputBase), 0x122e5c8
// (CConsoleVariable), 0x122e638 (CConsoleCommandBase), 0x122e65c (CConsoleClasslessCommand);
// member names and signatures from FableWin.pdb; enum values from Ego_r.pdb.
// Retail sizes: CConsoleVariable 0x10, CConsoleClasslessCommand 0x18 (its
// CConsoleCommandParameters is a 12-byte vector, the PDB's is 16), CConsole 0x9C.

#include <stddef.h>

#include "fable_counted_pointer.h"
#include "fable_singleton.h"
#include "fable_string.h"
#include "rebuild_abi.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_list.h"
#include "fable_rgb_colour.h"
#include "engine/CConsoleCommandLine.h"

enum EGenericVarType
{
    GENERIC_VAR_NULL = 0,
    GENERIC_VAR_SLONG = 1,
    GENERIC_VAR_ULONG = 2,
    GENERIC_VAR_FLOAT = 3,
    GENERIC_VAR_BOOL = 4,
    GENERIC_VAR_STRING = 5,
    GENERIC_VAR_GENERIC_VAR = 6
};

namespace _STL {
    template<> struct __type_traits<EGenericVarType> {
        typedef __true_type has_trivial_destructor;
        typedef __false_type is_POD_type;
    };
}

enum EInputKey
{
    KB_NULL = 0,
    KB_Y = 21,
    KB_A = 30,
    KB_HASH = 41,   // Ego_r.pdb EInputKey 0x29 (the console key; CMainGameComponent::Shutdown 0x004175E5)
    KB_B = 48,
    KB_UP = 109,
    KB_LEFT = 111,
    KB_RIGHT = 112,
    KB_DOWN = 114
};

class CConsole;
class CInputEvent;
class CFontBank;
class C2DBoxF;
class CGenericVar;

// Declared, not included: VC7.1's <vector> is not the retail container, and only the
// parameter type is needed here.
namespace std
{
    template<class T> class allocator;
    template<class T, class A> class vector;
}

typedef std::vector<CGenericVar, std::allocator<CGenericVar> > CGenericVarVector;

// A typed reference to the variable a console variable reads and writes.
class CGenericVarRef
{
public:
    CGenericVarRef(bool& value) : Type(GENERIC_VAR_BOOL) { PBool = &value; }
    CGenericVarRef(long& value) : Type(GENERIC_VAR_SLONG) { PSLong = &value; }
    CGenericVarRef(float& value) : Type(GENERIC_VAR_FLOAT) { PFloat = &value; }
    // FableWin ?PrintToString@CGenericVarRef@@QAEXAAVCCharString@@@Z.
    void PrintToString(CCharString& output);
    // Ego_r ?ReadFromParser@CGenericVarRef@@QAE_NAAVCStringParser@@@Z, retail 0x00A63FC0.
    bool ReadFromParser(class CStringParser& parser);

    EGenericVarType Type;
    union
    {
        bool* PBool;
        long* PSLong;
        float* PFloat;
        CCharString* PString;
    };
};

FABLE_STATIC_ASSERT(sizeof(CGenericVarRef) == 0x08);

class CConsoleInputBase
{
    friend class CConsoleInputTestAccess;
    // The guess/list helpers read GetFormattedName (slot 2) directly
    // (CConsole::SetGuessMode 0x009EAFF0 calls [vtbl+8]).
    friend class CConsole;
public:
    CConsoleInputBase(const CCharString& name) : Name(name) {}
    virtual ~CConsoleInputBase() {}
    const CCharString& GetName() const { return Name; }

protected:
    virtual bool ProcessCommandLine(const CCharString& line, CConsole& console) = 0;
    virtual CCharString GetFormattedName() const = 0;
    virtual CCharString GetTypeAsString() const = 0;
    virtual CCharString GetHelp() = 0;

    CCharString Name;
};

FABLE_STATIC_ASSERT(sizeof(CConsoleInputBase) == 0x08);

class CConsoleVariable : public CConsoleInputBase
{
    friend class CConsoleInputTestAccess;
public:
    CConsoleVariable(const CCharString& name, bool& value)
        : CConsoleInputBase(name), Var(value) {}
    CConsoleVariable(const CCharString& name, long& value)
        : CConsoleInputBase(name), Var(value) {}
    CConsoleVariable(const CCharString& name, float& value)
        : CConsoleInputBase(name), Var(value) {}
    virtual ~CConsoleVariable() {}

private:
    virtual bool ProcessCommandLine(const CCharString& line, CConsole& console);
    virtual CCharString GetFormattedName() const;
    virtual CCharString GetTypeAsString() const;
    virtual CCharString GetHelp();

    CGenericVarRef Var;
};

FABLE_STATIC_ASSERT(sizeof(CConsoleVariable) == 0x10);

// Retail layout: one 12-byte vector of parameter types.
class CConsoleCommandParameters
{
public:
    CConsoleCommandParameters() {}
    CConsoleCommandParameters(const CConsoleCommandParameters& other) : ParamTypes(other.ParamTypes) {}
    CConsoleCommandParameters(EGenericVarType type);
    CConsoleCommandParameters(EGenericVarType first, EGenericVarType second);
    CConsoleCommandParameters(EGenericVarType first, EGenericVarType second, EGenericVarType third);
    // Retail 0x009EBCB0.
    CConsoleCommandParameters(EGenericVarType first, EGenericVarType second, EGenericVarType third, EGenericVarType fourth);
    ~CConsoleCommandParameters();
    // FableWin ?GetParameterTypeAsString@CConsoleCommandParameters@@QBE?AVCCharString@@J@Z.
    CCharString GetParameterTypeAsString(long index) const;
    // Ego_r ?GetCorrectlyTypedParameter@CConsoleCommandParameters@@QBE?AVCGenericVar@@J@Z,
    // retail 0x009EA6C0: an empty CGenericVar of the parameter's type.
    CGenericVar GetCorrectlyTypedParameter(long index) const;

    _STL::vector<EGenericVarType> ParamTypes;

};

FABLE_STATIC_ASSERT(sizeof(CConsoleCommandParameters) == 0x0C);

// The console implementation keeps the single-parameter base constructor out
// of line; the startup registrar expands it. Preserve that unit boundary.
#ifndef FABLE_CONSOLE_PARAMETER_BASE_INLINE
#define FABLE_CONSOLE_PARAMETER_BASE_INLINE inline
#endif
class CConsoleCommandBase : public CConsoleInputBase
{
    friend class CConsoleInputTestAccess;
public:
    CConsoleCommandBase(const CCharString& name) : CConsoleInputBase(name) {}
    CConsoleCommandBase(const CCharString& name, const CConsoleCommandParameters& params)
        : CConsoleInputBase(name), ParamTypes(params) {}
    FABLE_CONSOLE_PARAMETER_BASE_INLINE CConsoleCommandBase(const CCharString& name, const EGenericVarType& type)
        : CConsoleInputBase(name), ParamTypes(type) {}
    virtual ~CConsoleCommandBase() {}

protected:
    virtual bool ProcessCommandLine(const CCharString& line, CConsole& console);
    virtual CCharString GetFormattedName() const;
    virtual CCharString GetTypeAsString() const;
    virtual CCharString GetHelp();
    virtual void CallCommandFunction(const CGenericVarVector& params) const = 0;

    CConsoleCommandParameters ParamTypes;
};

FABLE_STATIC_ASSERT(sizeof(CConsoleCommandBase) == 0x14);

typedef void (FABLE_FASTCALL *CConsoleCommandFunction)(const CGenericVarVector& params);

// A console command that calls a free function.
class CConsoleClasslessCommand : public CConsoleCommandBase
{
public:
    CConsoleClasslessCommand(
        const CCharString& name,
        CConsoleCommandFunction function,
        const EGenericVarType& type)
        : CConsoleCommandBase(name, type), CommandFunc(function) {}

private:
    virtual void CallCommandFunction(const CGenericVarVector& params) const;

    CConsoleCommandFunction CommandFunc;
};

FABLE_STATIC_ASSERT(sizeof(CConsoleClasslessCommand) == 0x18);

// FableWin's bound command carries a receiver and a fastcall free-function adapter.
template<class T> class CConsoleCommand : public CConsoleCommandBase {
public:
    typedef void (FABLE_FASTCALL *Function)(T&, const CGenericVarVector&);
    CConsoleCommand(const CCharString& name,T& receiver,Function function)
        : CConsoleCommandBase(name), Receiver(receiver), CommandFunc(function) {}
    __forceinline CConsoleCommand(const CCharString& name,T& receiver,Function function,const EGenericVarType& type)
        : CConsoleCommandBase(name,type), Receiver(receiver), CommandFunc(function) {}
    CConsoleCommand(const CCharString& name,T& receiver,Function function,const CConsoleCommandParameters& types)
        : CConsoleCommandBase(name,types), Receiver(receiver), CommandFunc(function) {}
    virtual void CallCommandFunction(const CGenericVarVector& params) const;
    T& Receiver;
    Function CommandFunc;
};

// RTTI .?AV?$CTBaseSingleton@VCConsole@@@@, vtable 0x0129C44C (slot 0 scalar
// deleting destructor 0x009EDB70): the console's singleton base is polymorphic,
// and ~CConsole 0x009ECF30 ends by restoring its vtable.
template<>
class CTBaseSingleton<CConsole>
{
public:
    virtual ~CTBaseSingleton() {}
    static CConsole& FABLE_FASTCALL Get();
    static void FABLE_FASTCALL Release();
};

class CConsole : public CTBaseSingleton<CConsole>
{
public:
    CConsole();
    virtual ~CConsole();

    void Initialise(char activateKeyChar, EInputKey activateKey, const CFontBank* font);
    void AddInput(CConsoleInputBase* input);
    void SendMessageToUser(const CCharString& message);
    static void FABLE_FASTCALL ConsoleAddStringBinding(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL ConsoleRunStringBinding(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL ConsoleAddKeyBinding(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL ConsoleRunScript(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL ConsoleConsoleListContainingString(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL ConsoleCommandList(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL ConsoleCommandListContainingString(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL ConsoleVariableList(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL ConsoleVariableListContainingString(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL SetConsoleTextColour(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL SetConsoleHistoryWindowColour(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL SetConsoleClientWindowColour(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL SetConsoleHelpWindowColour(CConsole&, const CGenericVarVector&);
    static void FABLE_FASTCALL SetConsoleScrollBarColour(CConsole&, const CGenericVarVector&);

    void RunScript(const CWideString& filename);
    void RunTextCommand(const CCharString& text_cmd); // retail 009EC710
    // FableWin ?ProcessInput@CConsole@@QAE_NABVCInputEvent@@@Z; retail 0x009EA530
    // (a `return false` stub; CInputProcessConsole::ProcessInput 0x00687F80 calls it).
    bool ProcessInput(const CInputEvent& event);
    // FableWin ?ProcessBindKeyInput@CConsole@@QAE_NABVCInputEvent@@@Z; retail 0x009EA540
    // (a `return false` stub; CInputProcessConsole::ProcessInput 0x00687F80 calls it
    // at 0x00688106 when the console is inactive).
    bool ProcessBindKeyInput(const CInputEvent& event);
    // FableWin ?Draw@CConsole@@QAEXABVC2DBoxF@@@Z; retail 0x009EA550 (an empty stub;
    // called at 0x00435C55 with the screen box while the console is active).
    void Draw(const C2DBoxF& box);
    bool IsActive() const { return Active; }

private:
    friend class CConsoleTestAccess;
    void AddCommandLineToHistory(const CConsoleCommandLine& line);
    // Ego_r private; retail 0x009ECA30 (Enter on the command line).
    void UserInputComplete();
    void ScrollToHistoryBeginning();
    // Ego_r.pdb private helpers. Callers inline the mode accessors (definitions
    // after the class); lib_console2.cpp keeps out-of-line copies
    // (CConsole_ModeHelpers_009ea340.cpp).
    bool IsGuessMode() const; // retail 009EA350
    bool IsHistoryMode() const; // retail 009EA340
    void UnsetGuessMode(); // retail 009EA580
    void UnsetHistoryMode(); // retail 009EA590
    CCharString GetHelpBarString() const; // retail 009EA560
    void ConfirmAlternative(); // retail 009EA600
    void ResetHistory(); // retail 009EAA40
    void ScrollUpThroughHistory(); // retail 009EABE0
    void ScrollDownThroughHistory(); // retail 009EAC30
    void ScrollToHistoryEnd(); // retail 009EAD10
    void CopyFrontBufferToBackBuffer(); // retail 009EA620
    void CopyBackBufferToFrontBuffer(); // retail 009EA640
    void ResetCommandLineBuffers(); // retail 009EA5A0
    void RevertToUserEntry(); // retail 009EA660
    void PrevInHistory(); // retail 009EA980
    void NextInHistory(); // retail 009EA9E0
    void SetHistoryMode(); // retail 009EAB80
    void SetGuessMode(); // retail 009EAFF0
    void NextCommandGuess(); // retail 009EB0D0
    CConsoleInputBase* GetRegisteredConsoleInput(const CCharString& key); // retail 009EB220
    // Ego_r private helpers, expanded into the static command bindings.
    void AddKeyBinding(char c, const CCharString& text);
    void AddStringBinding(const CCharString& str, const CCharString& text);
    void RunStringBinding(const CCharString& str);
    void RunCommandLine(); // retail 009EB430
    void SetHelpBarString(); // retail 009EB2C0
    _STL::map<char, CCharString> KeyBindingMap;
    _STL::map<CCharString, CCharString> StringBindingMap;
    CConsoleCommandLine FrontCommandLine;
    CConsoleCommandLine BackCommandLine;
    CCharString HelpBarString;
    _STL::map<CCharString, CCountedPointer<CConsoleInputBase> > RegisteredConsoleInputs;
    _STL::list<CConsoleCommandLine> CommandLineHistory;
    _STL::list<CConsoleCommandLine>::iterator VisibleHistoryStart;
    _STL::list<CConsoleCommandLine>::iterator VisibleHistoryFinish;
    long NumberOfHistoryLinesOnscreen;
    bool HistoryMode;
    _STL::list<CConsoleCommandLine>::iterator HistoryPos;
    bool MovingIntoHistory;
    bool GuessMode;
    _STL::map<CCharString, CCountedPointer<CConsoleInputBase> >::iterator GuessPos;
    char ToggleChar;
    EInputKey ToggleKey;
    const CFontBank* PFont;
    bool Active;
    long ConsoleAlpha;
    CRGBColour HistoryWindowColour;
    CRGBColour ClientWindowColour;
    CRGBColour HelpWindowColour;
    CRGBColour ScrollBarColour;
    CRGBColour TextColour;
    long CursorBlink;
    long CursorBlinkRate;
};

FABLE_STATIC_ASSERT(sizeof(CConsole) == 0x9C);

#ifndef FABLE_CONSOLE_MODE_OUT_OF_LINE
inline bool CConsole::IsGuessMode() const { return GuessMode; }
inline bool CConsole::IsHistoryMode() const { return HistoryMode; }
inline void CConsole::UnsetGuessMode() { GuessMode = false; }
inline void CConsole::UnsetHistoryMode() { HistoryMode = false; }
#endif

// Lazily created singleton. Retail keeps CTBaseSingleton<T>::PClass as a counted pointer;
// the CConsole one is g_FableConsolePClass_013CAA40.
extern CCountedPointer<CConsole> g_FableConsolePClass_013CAA40;

inline CConsole& FABLE_FASTCALL CTBaseSingleton<CConsole>::Get()
{
    if (g_FableConsolePClass_013CAA40.Data == 0)
        g_FableConsolePClass_013CAA40.Reset(new CConsole);
    return *g_FableConsolePClass_013CAA40.Data;
}

// FableWin: class CEngineMemoryInit (size 0x28).
class CEngineMemoryInit
{
public:
    long LandscapePoolSize;
    float LandscapePhysicalMemoryRatio;
    long LandscapeVirtualMemoryMinSize;
    long MeshPoolSize;
    long MeshStatsPoolSize;
    float MeshPhysicalMemoryRatio;
    long HiresTextureMemory;
    long PhysicsMeshPoolSize;
    long AnimationPoolSize;
    long ClothPoolSize;
};

FABLE_STATIC_ASSERT(sizeof(CEngineMemoryInit) == 0x28);

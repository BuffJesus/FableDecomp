#include "fable_console.h"
#include <stdio.h>
#include <string.h>

static char g_strings[128][256];
static int g_next_string;
static int g_type_calls;
static int g_print_calls;

static const char* text_of(const CCharString& value)
{
    unsigned long id = *reinterpret_cast<const unsigned long*>(&value);
    return id ? g_strings[id] : "";
}

static void set_text(CCharString& value, const char* text)
{
    unsigned long id = ++g_next_string;
    strcpy(g_strings[id], text);
    *reinterpret_cast<unsigned long*>(&value) = id;
}

CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const char* text, long) : storage_(0) { set_text(*this, text); }
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) {}
CCharString::~CCharString() {}
const CCharString& CCharString::operator=(const char* text)
{
    set_text(*this, text);
    return *this;
}
const CCharString& CCharString::operator+=(const char* text)
{
    char combined[256];
    strcpy(combined, text_of(*this));
    strcat(combined, text);
    set_text(*this, combined);
    return *this;
}
const CCharString& CCharString::operator+=(const CCharString& other)
{
    return *this += text_of(other);
}
CCharString FABLE_FASTCALL operator+(const CCharString& left, const char* right)
{
    CCharString result(left);
    result += right;
    return result;
}
CCharString FABLE_FASTCALL operator+(const char* left, const CCharString& right)
{
    CCharString result(left);
    result += right;
    return result;
}
void CGenericVarRef::PrintToString(CCharString& output)
{
    ++g_print_calls;
    output = "42";
}
bool CConsoleVariable::ProcessCommandLine(const CCharString&, CConsole&) { return false; }
CCharString CConsoleVariable::GetFormattedName() const { return CCharString("unused"); }
CCharString CConsoleVariable::GetTypeAsString() const
{
    ++g_type_calls;
    return CCharString("variable");
}

class HelpAccess : public CConsoleVariable
{
public:
    HelpAccess(bool& value) : CConsoleVariable(CCharString("name"), value) {}
    void SetType(EGenericVarType type)
    {
        *reinterpret_cast<EGenericVarType*>(reinterpret_cast<char*>(this) + 8) = type;
    }
};

class CConsoleInputTestAccess
{
public:
    static CCharString Call(CConsoleVariable& variable) { return variable.GetHelp(); }
};

int main()
{
    static const EGenericVarType types[] = {
        GENERIC_VAR_BOOL, GENERIC_VAR_FLOAT, GENERIC_VAR_SLONG,
        GENERIC_VAR_STRING, GENERIC_VAR_ULONG
    };
    static const char* expected[] = {
        "variable of type bool; value is currently 42",
        "variable of type float; value is currently 42",
        "variable of type slong; value is currently 42",
        "variable of type string; value is currently 42",
        "variable of type ; value is currently 42"
    };
    bool value = false;
    for (int index = 0; index != 5; ++index)
    {
        HelpAccess variable(value);
        variable.SetType(types[index]);
        CCharString result = CConsoleInputTestAccess::Call(variable);
        if (strcmp(text_of(result), expected[index]) != 0 ||
            g_type_calls != index + 1 || g_print_calls != index + 1)
            return index + 1;
    }
    puts("CONSOLE_VARIABLE_HELP_OK");
    return 0;
}

#include "fable_console.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char g_strings[128][256];
static int g_next_string;
static int g_type_calls;

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
CCharString& CCharString::operator=(const CCharString& other)
{
    storage_ = other.storage_;
    return *this;
}
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
void CCharString::Sprintf(const char* format, ...)
{
    char text[256];
    va_list arguments;
    va_start(arguments, format);
    vsprintf(text, format, arguments);
    va_end(arguments);
    set_text(*this, text);
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
    void SetRef(EGenericVarType type, void* pointer)
    {
        *reinterpret_cast<EGenericVarType*>(reinterpret_cast<char*>(this) + 8) = type;
        *reinterpret_cast<void**>(reinterpret_cast<char*>(this) + 12) = pointer;
    }
};

class CConsoleInputTestAccess
{
public:
    static CCharString Call(CConsoleVariable& variable) { return variable.GetHelp(); }
};

int main()
{
    static const char* expected[] = {
        "variable of type bool; value is currently TRUE",
        "variable of type bool; value is currently FALSE",
        "variable of type slong; value is currently -21",
        "variable of type float; value is currently 1.500000",
        "variable of type string; value is currently stored",
        "variable of type ; value is currently "
    };
    bool value = true;
    long integer = -21;
    float decimal = 1.5f;
    CCharString word("stored");
    for (int index = 0; index != 6; ++index)
    {
        HelpAccess variable(value);
        switch (index)
        {
        case 0: variable.SetRef(GENERIC_VAR_BOOL, &value); break;
        case 1: value = false; variable.SetRef(GENERIC_VAR_BOOL, &value); break;
        case 2: variable.SetRef(GENERIC_VAR_SLONG, &integer); break;
        case 3: variable.SetRef(GENERIC_VAR_FLOAT, &decimal); break;
        case 4: variable.SetRef(GENERIC_VAR_STRING, &word); break;
        default: variable.SetRef(GENERIC_VAR_ULONG, &integer); break;
        }
        CCharString result = CConsoleInputTestAccess::Call(variable);
        if (strcmp(text_of(result), expected[index]) != 0 ||
            g_type_calls != index + 1)
            return index + 1;
    }
    puts("CONSOLE_VARIABLE_HELP_PRINT_CHAIN_OK");
    return 0;
}

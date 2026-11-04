#include "fable_console.h"
#include <stdio.h>
#include <string.h>

static char g_strings[128][256];
static int g_next_string;
static int g_type_calls;
static int g_parameter_calls;

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
const CCharString& CCharString::operator=(const char* text)
{
    ++g_parameter_calls;
    set_text(*this, text);
    return *this;
}
CCharString FABLE_FASTCALL operator+(const CCharString& left, const char* right)
{
    CCharString result(left);
    result += right;
    return result;
}
CCharString FABLE_FASTCALL GFIntToCharString(long number)
{
    char digits[32];
    sprintf(digits, "%ld", number);
    return CCharString(digits);
}
CConsoleCommandParameters::~CConsoleCommandParameters() {}
bool CConsoleCommandBase::ProcessCommandLine(const CCharString&, CConsole&) { return false; }
CCharString CConsoleCommandBase::GetFormattedName() const { return CCharString("unused"); }
CCharString CConsoleCommandBase::GetTypeAsString() const
{
    ++g_type_calls;
    return CCharString("command");
}

class HelpAccess : public CConsoleCommandBase
{
public:
    HelpAccess() : CConsoleCommandBase(CCharString("name")) {}
    CCharString Call() { return GetHelp(); }
    void SetCount(int count)
    {
        static EGenericVarType types[2] = { GENERIC_VAR_SLONG, GENERIC_VAR_STRING };
        EGenericVarType** words = reinterpret_cast<EGenericVarType**>(&ParamTypes.ParamTypes);
        words[0] = types;
        words[1] = types + count;
        words[2] = types + 2;
    }
    ~HelpAccess()
    {
        EGenericVarType** words = reinterpret_cast<EGenericVarType**>(&ParamTypes.ParamTypes);
        words[0] = words[1] = words[2] = 0;
    }
private:
    void CallCommandFunction(const CGenericVarVector&) const {}
};

int main()
{
    static const char* expected[] = {
        "command with no parameters",
        "command with 1 parameter: slong ",
        "command with 2 parameters: slong string "
    };
    for (int count = 0; count != 3; ++count)
    {
        HelpAccess command;
        command.SetCount(count);
        CCharString result = command.Call();
        if (strcmp(text_of(result), expected[count]) != 0 ||
            g_type_calls != count + 1 || g_parameter_calls != count * (count + 1) / 2)
            return count + 1;
    }
    puts("CONSOLE_COMMAND_HELP_PARAMETER_CHAIN_OK");
    return 0;
}

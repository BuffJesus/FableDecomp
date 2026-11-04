#include "fable_console.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char g_strings[128][256];
static int g_next_string;
static int g_sprintf_calls;
static int g_copy_calls;

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
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) { ++g_copy_calls; }
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
void CCharString::Sprintf(const char* format, ...)
{
    char text[256];
    va_list arguments;
    va_start(arguments, format);
    vsprintf(text, format, arguments);
    va_end(arguments);
    ++g_sprintf_calls;
    set_text(*this, text);
}

static int check(CGenericVarRef& value, const char* expected)
{
    CCharString output("initial");
    value.PrintToString(output);
    return strcmp(text_of(output), expected);
}

int main()
{
    bool flag = true;
    long integer = -21;
    float decimal = 1.5f;
    CCharString word("stored");
    CGenericVarRef value(flag);
    value.Type = GENERIC_VAR_SLONG;
    value.PSLong = &integer;
    if (check(value, "-21") != 0 || g_sprintf_calls != 1) return 1;
    value.Type = GENERIC_VAR_FLOAT;
    value.PFloat = &decimal;
    if (check(value, "1.500000") != 0 || g_sprintf_calls != 2) return 2;
    value.Type = GENERIC_VAR_BOOL;
    value.PBool = &flag;
    if (check(value, "TRUE") != 0) return 3;
    flag = false;
    if (check(value, "FALSE") != 0) return 4;
    value.Type = GENERIC_VAR_STRING;
    value.PString = &word;
    int copies = g_copy_calls;
    if (check(value, "stored") != 0 || g_copy_calls != copies + 1) return 5;
    value.Type = GENERIC_VAR_NULL;
    if (check(value, "initial") != 0) return 6;
    value.Type = GENERIC_VAR_ULONG;
    if (check(value, "initial") != 0) return 7;
    value.Type = GENERIC_VAR_GENERIC_VAR;
    if (check(value, "initial") != 0) return 8;
    value.Type = static_cast<EGenericVarType>(-1);
    if (check(value, "initial") != 0) return 9;
    puts("GENERIC_VAR_REF_PRINT_OK");
    return 0;
}

#include "fable_string.h"
#include <stdio.h>
#include <string.h>

static CCharStringData g_existingData;
static int g_copyCalls;
static int g_appendCalls;
static int g_constructCalls;
static const char* g_appendText;
static unsigned long g_appendLength;
static const char* g_constructText;

void CCharString::MakeOwnCopyOfData()
{
    ++g_copyCalls;
}

template<> void CBasicString<char>::Append(const char* text, unsigned long length)
{
    ++g_appendCalls;
    g_appendText = text;
    g_appendLength = length;
}

template<> CBasicString<char>::CBasicString(const char* text)
{
    ++g_constructCalls;
    g_constructText = text;
    PData = const_cast<char*>(text);
    StringLength = static_cast<unsigned long>(strlen(text));
    DataLength = StringLength;
    UseFastExtend = true;
}

static CCharStringData* data_of(const CCharString& value)
{
    return *reinterpret_cast<CCharStringData* const*>(&value);
}

static int run_existing(const char* text, unsigned long expectedLength)
{
    unsigned char raw[sizeof(CCharString)];
    *reinterpret_cast<CCharStringData**>(raw) = &g_existingData;
    CCharString& value = *reinterpret_cast<CCharString*>(raw);
    g_copyCalls = g_appendCalls = g_constructCalls = 0;
    g_appendText = 0;
    const CCharString& returned = (value += text);
    return &returned == &value && data_of(value) == &g_existingData &&
           g_copyCalls == 1 && g_appendCalls == 1 &&
           g_appendText == text && g_appendLength == expectedLength &&
           g_constructCalls == 0 ? 0 : 1;
}

static int run_empty(const char* text, bool expectConstruction)
{
    unsigned char raw[sizeof(CCharString)];
    *reinterpret_cast<CCharStringData**>(raw) = 0;
    CCharString& value = *reinterpret_cast<CCharString*>(raw);
    g_copyCalls = g_appendCalls = g_constructCalls = 0;
    g_constructText = 0;
    const CCharString& returned = (value += text);
    CCharStringData* data = data_of(value);
    bool okay = &returned == &value && g_copyCalls == 0 && g_appendCalls == 0 &&
                g_constructCalls == (expectConstruction ? 1 : 0) &&
                (expectConstruction ? (data && data->owners == 1 && g_constructText == text) : !data);
    if (data != 0)
        delete data;
    return okay ? 0 : 1;
}

int main()
{
    if (run_existing("suffix", 6)) return 1;
    if (run_existing("", 0)) return 2;
    if (run_empty("new", true)) return 3;
    if (run_empty("", false)) return 4;
    if (run_empty(0, false)) return 5;
    puts("CHARSTRING_APPEND_LITERAL_OK");
    return 0;
}

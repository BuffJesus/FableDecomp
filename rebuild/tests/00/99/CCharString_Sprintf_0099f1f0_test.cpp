#include "fable_string.h"
#include <stdio.h>
#include <string.h>

static char g_captured[2048];
static int g_constructed;
static int g_released;
static CCharStringData g_oldData;

template<> CBasicString<char>::CBasicString(const char* text)
{
    ++g_constructed;
    strcpy(g_captured, text);
    PData = g_captured;
    StringLength = static_cast<unsigned long>(strlen(text));
    DataLength = StringLength;
    UseFastExtend = true;
}

void CCharString::UnassignString()
{
    ++g_released;
    CCharStringData*& data = *reinterpret_cast<CCharStringData**>(this);
    if (data != 0)
    {
        --data->owners;
        if (data != &g_oldData)
            delete data;
        data = 0;
    }
}

static CCharStringData* data_of(const CCharString& value)
{
    return *reinterpret_cast<CCharStringData* const*>(&value);
}

int main()
{
    unsigned char raw[sizeof(CCharString)];
    CCharString& value = *reinterpret_cast<CCharString*>(raw);
    *reinterpret_cast<CCharStringData**>(raw) = &g_oldData;
    g_oldData.owners = 1;
    value.Sprintf("%d/%s", -42, "oak");
    if (g_released != 1 || g_oldData.owners != 0 || g_constructed != 1 ||
        strcmp(g_captured, "-42/oak") != 0 || !data_of(value) ||
        data_of(value)->owners != 1)
        return 1;

    value.Sprintf("%s", "");
    if (g_released != 2 || g_constructed != 1 || data_of(value) != 0)
        return 2;

    char longText[1200];
    memset(longText, 'Q', sizeof(longText) - 1);
    longText[sizeof(longText) - 1] = 0;
    value.Sprintf("%s", longText);
    if (g_released != 2 || g_constructed != 2 || !data_of(value) ||
        strlen(g_captured) != 0x401 || g_captured[0] != 'Q' ||
        g_captured[0x400] != 'Q')
        return 3;

    value.Sprintf("%s", "");
    if (g_released != 3 || g_constructed != 2 || data_of(value) != 0)
        return 4;
    puts("CHARSTRING_SPRINTF_OK");
    return 0;
}

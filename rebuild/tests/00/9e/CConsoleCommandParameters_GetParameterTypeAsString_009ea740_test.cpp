#include "fable_console.h"
#include <stdio.h>
#include <string.h>

static const char* g_text;
CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_) {}
CCharString::~CCharString() {}
const CCharString& CCharString::operator=(const char* text)
{
    g_text = text;
    return *this;
}

int main()
{
    static const EGenericVarType values[] = {
        GENERIC_VAR_NULL, GENERIC_VAR_SLONG, GENERIC_VAR_ULONG,
        GENERIC_VAR_FLOAT, GENERIC_VAR_BOOL, GENERIC_VAR_STRING,
        GENERIC_VAR_GENERIC_VAR, static_cast<EGenericVarType>(-1),
        static_cast<EGenericVarType>(9)
    };
    static const char* expected[] = {
        "null", "slong", "ulong", "float", "bool", "string",
        "null", "null", "null"
    };
    unsigned char storage[sizeof(CConsoleCommandParameters)] = {0};
    EGenericVarType** vector = reinterpret_cast<EGenericVarType**>(storage);
    vector[0] = const_cast<EGenericVarType*>(values);
    vector[1] = vector[0] + sizeof(values) / sizeof(values[0]);
    vector[2] = vector[1];
    const CConsoleCommandParameters* parameters =
        reinterpret_cast<const CConsoleCommandParameters*>(storage);
    for (int index = 0; index != 9; ++index)
    {
        g_text = 0;
        CCharString result = parameters->GetParameterTypeAsString(index);
        if (!g_text || strcmp(g_text, expected[index]) != 0)
            return index + 1;
    }
    puts("PARAMETER_TYPE_STRING_OK");
    return 0;
}

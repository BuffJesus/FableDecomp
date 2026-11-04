#include "fable_console.h"
#include <stdio.h>
#include <string.h>

extern const CCharString g_013caa48;
fable_i32 g_CCharStringInstanceCount_013BD800;

static CCharStringData g_suffixData;
static CCharStringData g_nameData;
static CCharStringData g_joinedData;
static int g_suffixConstructions;
static int g_appendCalls;
static int g_unassignCalls;
static const CCharString* g_appendRight;
static CCharStringData* g_appendLeft;
static const char* g_suffixText;
static long g_suffixLength;

CCharString::CCharString(const char* text, long length) : storage_(&g_suffixData)
{
    ++g_CCharStringInstanceCount_013BD800;
    ++g_suffixConstructions;
    g_suffixText = text;
    g_suffixLength = length;
    g_suffixData.owners = 1;
}

const CCharString& CCharString::operator+=(const CCharString& right)
{
    ++g_appendCalls;
    g_appendRight = &right;
    g_appendLeft = storage_;
    if (storage_ != 0)
        --storage_->owners;
    storage_ = &g_joinedData;
    g_joinedData.owners = 1;
    return *this;
}

void CCharString::UnassignString()
{
    ++g_unassignCalls;
    if (storage_ != 0)
    {
        --storage_->owners;
        storage_ = 0;
    }
}

class FormatAccess : public CConsoleCommandBase
{
public:
    CCharString Call() const { return CConsoleCommandBase::GetFormattedName(); }
};

static CCharStringData* data_of(const CCharString& value)
{
    return *reinterpret_cast<CCharStringData* const*>(&value);
}

static int run_case(CCharStringData* nameData)
{
    unsigned char storage[sizeof(CConsoleCommandBase)];
    memset(storage, 0, sizeof(storage));
    *reinterpret_cast<CCharStringData**>(storage + 4) = nameData;
    FormatAccess* command = reinterpret_cast<FormatAccess*>(storage);
    g_nameData.owners = 1;
    g_appendCalls = g_unassignCalls = 0;
    g_appendRight = 0;
    g_appendLeft = 0;
    int countBefore = g_CCharStringInstanceCount_013BD800;
    {
        CCharString result = command->Call();
        if (data_of(result) != &g_joinedData || g_joinedData.owners != 1 ||
            g_appendCalls != 1 || g_appendRight != &g_013caa48 ||
            g_appendLeft != nameData || g_suffixData.owners != 1 ||
            (nameData && nameData->owners != 1) ||
            g_unassignCalls != 1 ||
            g_CCharStringInstanceCount_013BD800 != countBefore + 1)
            return 1;
    }
    return g_joinedData.owners == 0 && g_unassignCalls == 2 &&
           g_CCharStringInstanceCount_013BD800 == countBefore ? 0 : 2;
}

int main()
{
    if (g_suffixConstructions != 1 || !g_suffixText ||
        strcmp(g_suffixText, "(") != 0 || g_suffixLength != -1 ||
        data_of(g_013caa48) != &g_suffixData ||
        g_CCharStringInstanceCount_013BD800 != 1)
        return 1;
    if (run_case(&g_nameData) != 0) return 2;
    if (run_case(0) != 0) return 3;
    puts("CONSOLE_COMMAND_FORMATTED_CHAIN_OK");
    return 0;
}

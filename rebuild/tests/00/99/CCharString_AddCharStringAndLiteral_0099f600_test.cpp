#include "fable_string.h"
#include <new>
#include <stdio.h>
#include <string.h>

fable_i32 g_CCharStringInstanceCount_013BD800;
static int g_appends;
static int g_unassigns;
static const char* g_appendText;
static CCharStringData* g_appendLeftData;

const CCharString& CCharString::operator+=(const char* text)
{
    ++g_appends;
    g_appendText = text;
    g_appendLeftData = storage_;
    return *this;
}

void CCharString::UnassignString()
{
    ++g_unassigns;
    if (storage_ != 0)
    {
        --storage_->owners;
        storage_ = 0;
    }
}

static CCharStringData* data_of(const CCharString& value)
{
    return *reinterpret_cast<CCharStringData* const*>(&value);
}

static bool run_case(CCharStringData* leftData, const char* right)
{
    unsigned char leftStorage[sizeof(CCharString)];
    unsigned char outputStorage[sizeof(CCharString)];
    *reinterpret_cast<CCharStringData**>(leftStorage) = leftData;
    memset(outputStorage, 0xCD, sizeof(outputStorage));
    const CCharString& left = *reinterpret_cast<const CCharString*>(leftStorage);
    g_CCharStringInstanceCount_013BD800 = 0;
    g_appends = g_unassigns = 0;
    g_appendText = 0;
    g_appendLeftData = 0;
    long originalOwners = leftData ? leftData->owners : 0;
    CCharString* result = new (outputStorage) CCharString(operator+(left, right));
    return data_of(*result) == leftData &&
           (!leftData || leftData->owners == originalOwners + 1) &&
           g_appends == 1 && g_appendText == right &&
           g_appendLeftData == leftData && g_unassigns == 1 &&
           g_CCharStringInstanceCount_013BD800 == 1;
}

int main()
{
    CCharStringData shared;
    shared.owners = 1;
    if (!run_case(&shared, " (")) return 1;
    if (!run_case(0, "")) return 2;
    shared.owners = 1;
    if (!run_case(&shared, 0)) return 3;
    puts("FABLETLC_CCHARSTRING_STRING_PLUS_LITERAL_0099F600_PASS");
    return 0;
}

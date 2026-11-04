#include "fable_string.h"
#include <new>
#include <stdio.h>
#include <string.h>

fable_i32 g_CCharStringInstanceCount_013BD800;
static int g_appends;
static int g_unassigns;
static const CCharString* g_appendRight;
static CCharStringData* g_appendLeftData;

const CCharString& CCharString::operator+=(const CCharString& right)
{
    ++g_appends;
    g_appendRight = &right;
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

static void set_data(unsigned char* storage, CCharStringData* data)
{
    *reinterpret_cast<CCharStringData**>(storage) = data;
}

static bool run_case(CCharStringData* left_data, bool same_input)
{
    unsigned char left_storage[sizeof(CCharString)];
    unsigned char right_storage[sizeof(CCharString)];
    unsigned char output_storage[sizeof(CCharString)];
    set_data(left_storage, left_data);
    set_data(right_storage, 0);
    memset(output_storage, 0xCD, sizeof(output_storage));
    const CCharString& left = *reinterpret_cast<const CCharString*>(left_storage);
    const CCharString& right = same_input ? left : *reinterpret_cast<const CCharString*>(right_storage);
    g_CCharStringInstanceCount_013BD800 = 0;
    g_appends = g_unassigns = 0;
    g_appendRight = 0;
    g_appendLeftData = 0;
    long original_owners = left_data ? left_data->owners : 0;
    CCharString* result = new (output_storage) CCharString(operator+(left, right));
    return data_of(*result) == left_data &&
           (!left_data || left_data->owners == original_owners + 1) &&
           g_appends == 1 && g_appendRight == &right &&
           g_appendLeftData == left_data && g_unassigns == 1 &&
           g_CCharStringInstanceCount_013BD800 == 1;
}

int main()
{
    CCharStringData shared;
    shared.owners = 1;
    if (!run_case(&shared, false)) return 1;
    shared.owners = 1;
    if (!run_case(&shared, true)) return 2;
    if (!run_case(0, false)) return 3;
    puts("FABLETLC_CCHARSTRING_STRING_PLUS_STRING_0099F570_PASS");
    return 0;
}

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_string.h"

fable_i32 g_CCharStringInstanceCount_013BD800;
char g_FableEmptyNarrowString_0129AAF4[1] = {0};
static int substringCalls;
static int storageLive;

static CCharStringData* makeData(const char* text, unsigned long length)
{
    CCharStringData* data = static_cast<CCharStringData*>(malloc(sizeof(CCharStringData)));
    if (!data) return 0;
    data->Data.PData = static_cast<char*>(malloc(length + 1));
    if (!data->Data.PData) { free(data); return 0; }
    memcpy(data->Data.PData, text, length);
    data->Data.PData[length] = 0;
    data->Data.StringLength = length;
    data->Data.DataLength = length + 1;
    data->Data.UseFastExtend = true;
    data->owners = 1;
    ++storageLive;
    return data;
}

CCharString::CCharString() : storage_(0) { ++g_CCharStringInstanceCount_013BD800; }
CCharString::CCharString(const char* text, long) : storage_(0)
{
    ++g_CCharStringInstanceCount_013BD800;
    if (text && *text) storage_ = makeData(text, static_cast<unsigned long>(strlen(text)));
}
CCharString::CCharString(const CCharString& other) : storage_(other.storage_)
{
    ++g_CCharStringInstanceCount_013BD800;
    if (storage_) ++storage_->owners;
}
void CCharString::UnassignString()
{
    if (storage_ && --storage_->owners == 0)
    {
        free(storage_->Data.PData);
        free(storage_);
        --storageLive;
    }
    storage_ = 0;
}
inline CCharString::~CCharString()
{
    UnassignString();
    --g_CCharStringInstanceCount_013BD800;
}
CCharString CCharString::GetSubString(unsigned long first, unsigned long last) const
{
    ++substringCalls;
    CCharString result;
    if (storage_ && last > first && first < storage_->Data.StringLength)
        result.storage_ = makeData(storage_->Data.PData + first, last - first);
    return result;
}
static void requireTrim(bool valid)
{
    if (!valid) { puts("TRIM_FIXTURE_FAIL"); exit(1); }
}

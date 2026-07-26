#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

struct CWideStringData
{
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#pragma pack(push, 1)
struct CCharStringData
{
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#pragma pack(pop)

class CWideString
{
public:
    CWideString();
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);

private:
    CWideStringData* storage_;
};

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();

private:
    void UnassignString();
    CCharStringData* AllocStringData(
        const char* text,
        long length);

    CCharStringData* storage_;
};

// Retail folds equivalent narrow/wide string lifetime helpers. These counters
// remain separate because their addresses and object families are distinct.
extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

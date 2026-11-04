// Behaviour fixture for the C3DAnimFileSequenceChunk setters (provider C3DAnimFileSequenceChunk_Setters_00aaf570.cpp).
// The chunk's constructor is not linked, so it is raw storage laid out like retail (BoneIndex +0x2C, ParentIndex +0x30,
// BoneName +0x34, SamplesPerSecond +0x38). The fixture supplies a recording CCharString::operator=: SetBoneName must
// assign into the chunk's BoneName from the caller's string; each other setter must write only its own field.
#include <stdio.h>
#include <string.h>
#include "fable_chunked_file.h"

static int g_fail, g_assigns;
static const void* g_assign_this;
static const void* g_assign_from;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CCharString& CCharString::operator=(const CCharString& other)
{
    ++g_assigns; g_assign_this = this; g_assign_from = &other;
    return *this;
}

int main()
{
    unsigned char raw[0x50];
    for (int i = 0; i < 0x50; ++i) raw[i] = (unsigned char)(0x40 + i);
    unsigned char before[0x50];
    memcpy(before, raw, sizeof(raw));
    C3DAnimFileSequenceChunk* chunk = reinterpret_cast<C3DAnimFileSequenceChunk*>(raw);

    chunk->SetBoneIndex(17);
    chunk->SetParentIndex(-1);
    chunk->SetSamplesPerSecond(29.97f);
    long boneIndex, parentIndex; float rate;
    memcpy(&boneIndex, raw + 0x2C, 4); memcpy(&parentIndex, raw + 0x30, 4); memcpy(&rate, raw + 0x38, 4);
    CHECK(boneIndex == 17 && parentIndex == -1 && rate == 29.97f);
    CHECK(memcmp(raw, before, 0x2C) == 0 && memcmp(raw + 0x34, before + 0x34, 4) == 0 && memcmp(raw + 0x3C, before + 0x3C, 0x14) == 0);

    const CCharString* name = reinterpret_cast<const CCharString*>(0x1234);
    chunk->SetBoneName(*name);
    CHECK(g_assigns == 1 && g_assign_this == raw + 0x34 && g_assign_from == name);

    if (g_fail) { printf("ANIM_SEQUENCE_SETTERS_FAIL %d\n", g_fail); return 1; }
    printf("ANIM_SEQUENCE_SETTERS_OK\n");
    return 0;
}

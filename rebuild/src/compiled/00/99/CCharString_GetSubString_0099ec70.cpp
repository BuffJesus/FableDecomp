#include <cstddef>

extern "C" long g_charStringRefTrace; // 0x013bd800
extern "C" char g_emptyCharStringData; // 0x0129aaf4 (empty-string storage byte)

struct CCharStringData
{
    char* data;
    long length;
    long refCount; // real field is a byte at +0xd inside a larger header; kept opaque via byte-offset write below
};

struct CCharString
{
    CCharStringData* storage;

    CCharStringData* AllocStringData(char* data, long len);
    void UnassignString();
    CCharString* GetSubString(CCharString* retVal, unsigned long start, unsigned long len);
};

// Real implementations live at 0x0099ea60 (AllocStringData) / 0x0099e9b0 (UnassignString);
// they are separate retail functions and are intentionally left undefined here (external
// callees only) so this translation unit authors ONLY GetSubString itself.

// CCharString GetSubString(CCharString* this, ulong start, ulong len)
// Hidden-return-pointer thiscall: ecx = this, [esp+4]=retptr, [esp+8]=start, [esp+0xc]=len
CCharString* CCharString::GetSubString(CCharString* retVal, unsigned long start, unsigned long len)
{
    CCharStringData* newStorage = 0;
    CCharString localTmp;

    ++g_charStringRefTrace;

    CCharStringData* storage = this->storage;
    localTmp.storage = newStorage;
    char* base;
    if (!storage)
        base = &g_emptyCharStringData;
    else
        base = storage->data;

    char* subPtr = base + start;
    if (subPtr == 0)
        goto skipAlloc;
    if (*subPtr == 0)
        goto skipAlloc;
    {
        long subLen = (long)len - (long)start;
        newStorage = localTmp.AllocStringData(subPtr, subLen);
        localTmp.storage = newStorage;
    }
skipAlloc:

    retVal->storage = 0;
    ++g_charStringRefTrace;

    if ((void*)&localTmp != (void*)retVal)
    {
        if (retVal->storage != 0)
        {
            retVal->UnassignString();
        }
        if (newStorage != 0)
        {
            retVal->storage = newStorage;
            ++*((long*)((char*)newStorage + 0xd));
        }
    }

    localTmp.UnassignString();
    --g_charStringRefTrace;

    return retVal;
}
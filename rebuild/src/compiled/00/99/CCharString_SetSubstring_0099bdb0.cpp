extern "C" long g_charStringRefTrace; // 0x013bca20
extern "C" unsigned short g_emptyWideStringData; // 0x00129a8e0 (empty wide-string storage code unit)
extern "C" void __cdecl free_00bfea14(void* p);      // 0x00bfea14 (free thunk)
extern "C" void __cdecl operator_delete_00bfe9bc(void* p); // 0x00bfe9bc (operator delete)

struct CWideStringData
{
    unsigned short* data;
    long unused1;
    long unused2;
    long refCount; // offset +0xC
};

struct CCharString
{
    CWideStringData* storage;

    CWideStringData* AllocStringData(unsigned short* s, long n);
    void UnassignString();
    CCharString* SetSubstring(CCharString* retVal, unsigned long start, unsigned long len);
};

// Real implementations live at 0x0099b3c0 (AllocStringData) / 0x0099b4d0 (UnassignString);
// they are separate retail functions and are intentionally left undefined here (external
// callees only) so this translation unit authors ONLY SetSubstring itself.

// CCharString SetSubstring(CCharString* this, ulong start, ulong len)
// Hidden-return-pointer thiscall: ecx = this, [esp+4]=retptr, [esp+8]=start, [esp+0xc]=len
CCharString* CCharString::SetSubstring(CCharString* retVal, unsigned long start, unsigned long len)
{
    CWideStringData* newStorage = 0;
    CCharString localTmp;

    ++g_charStringRefTrace;

    CWideStringData* storage = this->storage;
    unsigned short* base;
    if (!storage)
        base = &g_emptyWideStringData;
    else
        base = storage->data;

    unsigned short* subPtr = base + start;
    if (subPtr == 0)
        goto skipAlloc;
    if (*subPtr == 0)
        goto skipAlloc;
    {
        long subLen = (long)len - (long)start;
        newStorage = localTmp.AllocStringData(subPtr, subLen);
    }
skipAlloc:

    retVal->storage = 0;
    ++g_charStringRefTrace;

    if ((void*)&localTmp != (void*)retVal)
    {
        CWideStringData* oldRetStorage = retVal->storage;
        if (newStorage != oldRetStorage)
        {
            if (oldRetStorage != 0)
            {
                retVal->UnassignString();
            }
            if (newStorage != 0)
            {
                retVal->storage = newStorage;
                ++newStorage->refCount;
            }
        }
    }

    if (newStorage != 0)
    {
        if (--newStorage->refCount <= 0)
        {
            if (newStorage->data != 0)
                free_00bfea14(newStorage->data);
            operator_delete_00bfe9bc(newStorage);
        }
    }
    --g_charStringRefTrace;

    return retVal;
}
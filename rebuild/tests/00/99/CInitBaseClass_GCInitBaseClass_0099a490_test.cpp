#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_init_base_class.h"

// The compiled vtables, reached without spelling a mangled identifier.
#pragma comment(linker, "/alternatename:_g_testCInitBaseClassVTable=??_7CInitBaseClass@@6B@")
#pragma comment(linker, "/alternatename:_g_testCBaseVTable=??_7CBaseClass@@6B@")
extern "C" void* g_testCInitBaseClassVTable[];
extern "C" void* g_testCBaseVTable[];

long g_CInitBaseClassConstructed_013BCA18 = 0;
long g_CInitBaseClassValid_013BCA14 = 0;
void CInitBaseClass::Validate() const {}

static void* g_deleted;
static int g_deleteCalls;
void operator delete(void* p)
{
    g_deleted = p;
    ++g_deleteCalls;
}

// Calls vtable slot 0 (the scalar deleting destructor) with an explicit flag.
struct SlotCaller
{
    void* Call(unsigned int flags);
};
typedef void* (SlotCaller::*SlotMethod)(unsigned int);

static void* CallScalarDelete(void* object, unsigned int flags)
{
    union
    {
        void* raw;
        SlotMethod method;
    } slot;
    slot.raw = (*reinterpret_cast<void***>(object))[0];
    return (reinterpret_cast<SlotCaller*>(object)->*slot.method)(flags);
}

int main()
{
    if (g_testCInitBaseClassVTable[1] == 0) return 1;  // Validate follows

    // delete through the virtual destructor: flag 1 frees this object.
    unsigned char* heap = static_cast<unsigned char*>(malloc(8));
    *reinterpret_cast<void**>(heap) = g_testCInitBaseClassVTable;
    heap[4] = 1;
    g_CInitBaseClassValid_013BCA14 = 6;
    delete reinterpret_cast<CInitBaseClass*>(heap);
    if (g_deleteCalls != 1 || g_deleted != heap) return 2;
    if (g_CInitBaseClassValid_013BCA14 != 5 || heap[4] != 0) return 3;
    if (*reinterpret_cast<void**>(heap) != g_testCBaseVTable) return 4;
    free(heap);

    // Flag 0 destroys in place, returns this and frees nothing.
    unsigned char storage[8];
    memset(storage, 0x11, sizeof(storage));
    *reinterpret_cast<void**>(storage) = g_testCInitBaseClassVTable;
    storage[4] = 0;
    if (CallScalarDelete(storage, 0) != storage) return 5;
    if (g_deleteCalls != 1 || g_CInitBaseClassValid_013BCA14 != 5) return 6;
    if (*reinterpret_cast<void**>(storage) != g_testCBaseVTable) return 7;
    if (storage[5] != 0x11) return 8;
    puts("FABLETLC_INITBASE_SCALAR_DELETE PASS");
    return 0;
}
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_base_class.h"

// The compiled vtables, reached without spelling a mangled identifier.
#pragma comment(linker, "/alternatename:_g_testCBaseClassVTable=??_7CBaseClass@@6B@")
#pragma comment(linker, "/alternatename:_g_testNonCopyableVTable=??_7CBaseClassNonCopyable@@6B@")
extern "C" void* g_testCBaseClassVTable[];
extern "C" void* g_testNonCopyableVTable[];

static void* g_deleted;
static int g_deletes;
void operator delete(void* p)
{
    g_deleted = p;
    ++g_deletes;
}

int main()
{
    // delete through vtable slot 0 (flag 1) frees; the object ends on the root table.
    unsigned char* heap = static_cast<unsigned char*>(malloc(8));
    *reinterpret_cast<void**>(heap) = g_testNonCopyableVTable;
    delete reinterpret_cast<CBaseClassNonCopyable*>(heap);
    if (g_deletes != 1 || g_deleted != heap) return 1;
    if (*reinterpret_cast<void**>(heap) != g_testCBaseClassVTable) return 2;
    free(heap);
    puts("FABLETLC_NONCOPYABLE_SCALAR_DELETE PASS");
    return 0;
}

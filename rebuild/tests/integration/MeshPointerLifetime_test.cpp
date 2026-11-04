#include "fable_counted_pointer.h"
#include <stdio.h>

class CMeshDataBank;
typedef CCountedPointer<CMeshDataBank> Pointer;
static Pointer* g_dest;
static Pointer* g_source;
static CCPPointerInfo* g_other;
static int g_mode, g_callbacks;
static void* g_last_data;
static char g_data[4][16];

static void __fastcall DeleteBoundary(void* data)
{
    ++g_callbacks;
    g_last_data = data;
    if (g_mode == 1)
    {
        g_source->Data = (CMeshDataBank*)g_data[3];
        g_source->Info = g_other;
    }
    if (g_mode == 2)
    {
        g_dest->Data = (CMeshDataBank*)g_data[3];
        g_dest->Info = g_other;
    }
}

static int Run(int method, int old_present, int new_selector,
               int count_index, int new_count_index, int mode)
{
    const unsigned long counts[] = {0, 1, 2, 0xffffffffUL};
    union { double align; char bytes[sizeof(Pointer)]; } destStore, sourceStore;
    Pointer* dest = (Pointer*)&destStore;
    Pointer* source = (Pointer*)&sourceStore;
    CCPPointerInfo* oldInfo = new CCPPointerInfo(&DeleteBoundary, g_data[1]);
    CCPPointerInfo* newInfo = new CCPPointerInfo(&DeleteBoundary, g_data[2]);
    CCPPointerInfo* other = new CCPPointerInfo(&DeleteBoundary, g_data[3]);
    oldInfo->RefCount = counts[count_index];
    newInfo->RefCount = counts[new_count_index];
    CCPPointerInfo* initialOld = old_present ? oldInfo : 0;
    CCPPointerInfo* requested = new_selector == 0 ? 0 :
        new_selector == 1 ? initialOld : newInfo;
    dest->Data = (CMeshDataBank*)g_data[1];
    dest->Info = initialOld;
    source->Data = (CMeshDataBank*)g_data[2];
    source->Info = requested;
    g_dest = dest;
    g_source = source;
    g_other = other;
    g_mode = mode;
    g_callbacks = 0;
    g_last_data = 0;
    bool changing = method == 0 ? bool(initialOld) : initialOld != requested;
    bool deleted = changing && initialOld && counts[count_index] == 1;
    if (method == 0) dest->ReleaseData();
    else dest->ShareData((CMeshDataBank*)g_data[2], requested);
    CCPPointerInfo* expectedInfo = method == 0 ? 0 : changing ? requested : initialOld;
    CMeshDataBank* expectedData = method == 0 ? 0 :
        changing ? (CMeshDataBank*)g_data[2] : (CMeshDataBank*)g_data[1];
    if (dest->Data != expectedData || dest->Info != expectedInfo) return 1;
    if (g_callbacks != int(deleted) || (deleted && g_last_data != g_data[1])) return 2;
    if (changing && initialOld)
    {
        unsigned long expected = counts[count_index] - 1;
        if (mode == 2 && deleted && oldInfo->RefCount != expected) return 3;
        if (!deleted && oldInfo->RefCount != expected) return 4;
    }
    else if (oldInfo->RefCount != counts[count_index]) return 5;
    if (requested == newInfo && newInfo->RefCount !=
        counts[new_count_index] + (method != 0 && changing ? 1UL : 0UL)) return 6;
    if (deleted && mode == 1 && source->Info != other) return 7;
    bool oldFreed = deleted && mode != 2;
    bool otherFreed = deleted && mode == 2;
    dest->Info = 0;
    source->Info = 0;
    if (!oldFreed) delete oldInfo;
    delete newInfo;
    if (!otherFreed) delete other;
    return 0;
}

int main()
{
    int cases = 0;
    for (int method = 0; method < 2; ++method)
    for (int old = 0; old < 2; ++old)
    for (int next = 0; next < 3; ++next)
    for (int a = 0; a < 4; ++a)
    for (int b = 0; b < 4; ++b)
    for (int mode = 0; mode < 3; ++mode)
    {
        int result = Run(method, old, next, a, b, mode);
        if (result)
        {
            printf("MESH_POINTER_LIFETIME_FAIL method=%d old=%d next=%d a=%d b=%d mode=%d check=%d\n",
                   method, old, next, a, b, mode, result);
            return result;
        }
        ++cases;
    }
    printf("MESH_POINTER_LIFETIME_BEHAVIOR_PASS cases=%d\n", cases);
    return 0;
}

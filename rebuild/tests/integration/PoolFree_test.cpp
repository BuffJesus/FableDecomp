#include "fable_handle_memory_pool.h"
#include "fable_memory_allocator.h"
#include <stdio.h>
#include <string.h>
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);
typedef CHandleBasedMemoryPool Pool;
typedef Pool::CHeader Header;
static union { double align; char bytes[0x464]; } memory;
static Header areas[3], extras[2];
static Pool::CHandleInfo handles[16];
static void Insert(Pool* pool, Header* h)
{
    unsigned long bucket = (unsigned long)h->Size / 512; if (bucket > 255) bucket = 255;
    Header** slot = &pool->FreeLists[bucket];
    h->RefAvailable = slot; h->NextAvailable = *slot;
    if (*slot) (*slot)->RefAvailable = &h->NextAvailable;
    *slot = h;
}
int main()
{
    const long sizes[] = { 100, 512 * 7 + 3, 512 * 300 }, neighbours[] = { 300, 0, 512 * 9 + 5 };
    const short paddings[] = { 0, 12 };
    const long slots[] = { 0, 5, 15 }, firsts[] = { -1, 3 }, largests[] = { 0, 0x7fffffff };
    int cases = 0;
    for (int multi = 0; multi < 2; ++multi) for (int prev = 0; prev < 3; ++prev) for (int next = 0; next < 3; ++next)
    for (int z = 0; z < 3; ++z) for (int p = 0; p < 2; ++p) for (int s = 0; s < 3; ++s) for (int f = 0; f < 2; ++f)
    for (int extra = 0; extra < 3; extra += 2) for (int l = 0; l < 2; ++l) {
        memset(&memory, 0x5a, sizeof(memory));
        Pool* pool = (Pool*)&memory;
        memset(pool->FreeLists, 0, sizeof(pool->FreeLists));
        pool->MultiThreaded = multi != 0; pool->AllocatedSize = 0x100000; pool->NumAllocations = 7;
        pool->LargestFreeArea = largests[l]; pool->FirstUnusedHandle = firsts[f];
        pool->Handles._M_start = handles;
        if (multi) InitializeCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(pool->CriticalSection));
        for (int i = 0; i < 16; ++i) handles[i].NextUnusedHandle = 0x700 + i;
        memset(areas, 0x3c, sizeof(areas)); memset(extras, 0x2d, sizeof(extras));
        const int state[3] = { prev, 2, next }; // 0 none, 1 free, 2 allocated
        for (int i = 0; i < 3; ++i) {
            Header& h = areas[i];
            h.Handle = i; h.Size = i == 1 ? sizes[z] : neighbours[i];
            h.Padding = i == 1 ? paddings[p] : state[i] == 1 ? 0 : (short)(4 * i);
            h.Status = state[i] == 1 ? Pool::STATUS_FREE : Pool::STATUS_ALLOCATED; h.Locked = 1;
            h.RefAvailable = 0; h.NextAvailable = 0;
            h.PrevContiguous = i > 0 && state[i - 1] ? &areas[i - 1] : 0;
            h.NextContiguous = i < 2 && state[i + 1] ? &areas[i + 1] : 0;
        }
        handles[slots[s]].Header = &areas[1];
        for (int k = 0; k < extra; ++k) {
            extras[k].Size = k ? 300 : sizes[z]; extras[k].Padding = 0; extras[k].Status = Pool::STATUS_FREE; Insert(pool, &extras[k]);
        }
        if (prev == 1) Insert(pool, &areas[0]);
        if (next == 1) Insert(pool, &areas[2]);
        pool->Free(slots[s]);
        if (multi) DeleteCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(pool->CriticalSection));
        Header* survivor = prev == 1 ? &areas[0] : &areas[1];
        long total = sizes[z] + paddings[p] + (prev == 1 ? neighbours[0] + 32 : 0) + (next == 1 ? neighbours[2] + 32 : 0);
        unsigned long bucket = (unsigned long)total / 512; if (bucket > 255) bucket = 255;
        if (pool->FirstUnusedHandle != slots[s] || handles[slots[s]].NextUnusedHandle != firsts[f]) return 1;
        if (pool->NumAllocations != 8 || pool->AllocatedSize != 0x100000 - (sizes[z] + paddings[p] + 32)) return 2;
        if (survivor->Size != total || survivor->Status != Pool::STATUS_FREE || survivor->Padding) return 3;
        if (pool->FreeLists[bucket] != survivor || survivor->RefAvailable != &pool->FreeLists[bucket]) return 4;
        if (pool->LargestFreeArea != (largests[l] > total ? largests[l] : total)) return 5;
        // Absorbed areas leave every free list; the survivor's links bracket the merged span.
        for (int i = 0; i < 3; ++i) if (&areas[i] != survivor && state[i] && (i == 1 || state[i] == 1)) {
            if (areas[i].Status != Pool::STATUS_NULL || areas[i].RefAvailable || areas[i].NextAvailable) return 6;
        }
        if (survivor->NextContiguous != (next == 2 ? &areas[2] : 0)) return 7;
        if (next == 2 && areas[2].PrevContiguous != survivor) return 8;
        if (survivor->PrevContiguous != (prev == 2 ? &areas[0] : 0)) return 9;
        int listed = 0;
        for (int b = 0; b < 256; ++b) for (Header* h = pool->FreeLists[b]; h; h = h->NextAvailable) {
            if (h->Status != Pool::STATUS_FREE) return 10;
            ++listed;
        }
        if (listed != extra + 1) return 11;
        ++cases;
    }
    printf("POOL_FREE_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}

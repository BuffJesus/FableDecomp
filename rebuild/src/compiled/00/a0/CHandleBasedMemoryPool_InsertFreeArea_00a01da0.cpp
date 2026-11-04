#include "fable_handle_memory_pool.h"
void CHandleBasedMemoryPool::InsertFreeArea(CHeader* header)
{
    header->Size += header->Padding;
    header->Padding = 0;
    unsigned long index = (unsigned long)header->Size / FREE_LIST_SIZE_STEP;
    if (index >= FREE_LIST_COUNT - 1)
        index = FREE_LIST_COUNT - 1;
    CHeader** list = &FreeLists[index];
    header->RefAvailable = list;
    CHeader* next = *list;
    header->NextAvailable = next;
    if (next)
        next->RefAvailable = &header->NextAvailable;
    *list = header;
    if (LargestFreeArea < header->Size)
        LargestFreeArea = header->Size;
}

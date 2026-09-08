#include <stdio.h>
#include "engine/CResource.h"

void __fastcall CResource_ListRemove(CResource* resource);

static void Clear(CResource& resource)
{
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&resource);
    for (unsigned int i = 0; i < sizeof(resource); ++i)
        bytes[i] = 0;
}

int main()
{
    CResource previous;
    CResource current;
    CResource next;
    CResource outsider;
    Clear(previous);
    Clear(current);
    Clear(next);
    Clear(outsider);

    previous.NextResource = &current;
    current.PrevResource = &previous;
    current.NextResource = &next;
    next.PrevResource = &current;
    CResource_ListRemove(&current);
    if (previous.NextResource != &next || next.PrevResource != &previous ||
        current.PrevResource != 0 || current.NextResource != 0)
        return 1;

    current.PrevResource = &previous;
    current.NextResource = &next;
    previous.NextResource = &outsider;
    next.PrevResource = &current;
    CResource_ListRemove(&current);
    if (previous.NextResource != &outsider || next.PrevResource != &current ||
        current.PrevResource != 0 || current.NextResource != 0)
        return 2;

    current.PrevResource = &previous;
    current.NextResource = 0;
    CResource_ListRemove(&current);
    if (current.PrevResource != &previous || current.NextResource != 0)
        return 3;

    printf("RESOURCE_LIST_REMOVE PASS\n");
    return 0;
}

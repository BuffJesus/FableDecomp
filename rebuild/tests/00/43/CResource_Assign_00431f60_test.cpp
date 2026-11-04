#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>

int main()
{
    unsigned char targetBytes[sizeof(CResource)];
    unsigned char sourceBytes[sizeof(CResource)];
    CResource* target = reinterpret_cast<CResource*>(targetBytes);
    CResource* source = reinterpret_cast<CResource*>(sourceBytes);
    for (int i = 0; i < 16; ++i) {
        memset(targetBytes, 0xa5, sizeof(targetBytes));
        memset(sourceBytes, 0x5a, sizeof(sourceBytes));
        source->IVRefCount = 11 + i;
        source->ResourceList = reinterpret_cast<CResourceList*>(0x1000 + 16 * i);
        source->PrevResource = reinterpret_cast<CResource*>(0x2000 + 16 * i);
        source->NextResource = reinterpret_cast<CResource*>(0x3000 + 16 * i);
        source->ResourceSize = 100 + i;
        source->LastUsedFrame = 200 + i;
        CResource& result = target->CResource::operator=(*source);
        if (&result != target || memcmp(targetBytes + 4, sourceBytes + 4, 24) != 0) return 1;
        for (int j = 0; j < 4; ++j) if (targetBytes[j] != 0xa5) return 2;
    }
    puts("RESOURCE_ASSIGN_PASS cases=16");
    return 0;
}

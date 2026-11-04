// CResource::SetAsUnloadable (0x009FBF50): back-date LastUsedFrame to the list's frame minus its
// unload delay; no-op without a list. Drives the landed member on raw storage (no constructors).
#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>

int main()
{
    union { double align; unsigned char bytes[sizeof(CResourceList)]; } listStorage;
    union { double align; unsigned char bytes[sizeof(CResource)]; } resourceStorage;
    memset(&listStorage, 0, sizeof(listStorage));
    memset(&resourceStorage, 0, sizeof(resourceStorage));
    CResourceList* list = reinterpret_cast<CResourceList*>(listStorage.bytes);
    CResource* resource = reinterpret_cast<CResource*>(resourceStorage.bytes);
    list->CurrentFrame = 30;
    list->SetResourceUnloadDelay(8);
    resource->ResourceList = list;
    resource->SetAsUnloadable();
    if (resource->LastUsedFrame != 22) { printf("009fbf50_TEST FAIL\n"); return 1; }
    resource->ResourceList = 0;
    resource->LastUsedFrame = 5;
    resource->SetAsUnloadable();
    if (resource->LastUsedFrame != 5) { printf("009fbf50_TEST FAIL\n"); return 1; }
    printf("009fbf50_TEST PASS\n");
    return 0;
}

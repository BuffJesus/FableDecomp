#include <stdio.h>
#include <string.h>
#include "candidates/landscape_renderer_release_compiled.h"

CEngineLandscapeMapList* DAT_01436e8c = 0;
static int g_releaseCount = 0;

void CEngineLandscapeMap::ReleaseDefaultPoolSurfaces()
{
    ++g_releaseCount;
}

int main()
{
    CEngineLandscapeRenderer renderer;
    CEngineLandscapeMapList list;
    CEngineLandscapeMap map;
    CEngineLandscapeMapEntry emptyEntry;
    CEngineLandscapeMapEntry mapEntry;
    CEngineLandscapeMapEntry* entries[3];
    memset(&list, 0, sizeof(list));
    emptyEntry.LandscapeMap = 0;
    mapEntry.LandscapeMap = &map;
    entries[0] = 0;
    entries[1] = &emptyEntry;
    entries[2] = &mapEntry;
    list.Begin = entries;
    list.End = entries + 3;
    DAT_01436e8c = &list;
    renderer.OnReleaseDefaultPoolResources();
    if (g_releaseCount != 1) return 1;
    printf("LANDSCAPE_RENDERER_RELEASE_POOL_TEST PASS\n");
    return 0;
}

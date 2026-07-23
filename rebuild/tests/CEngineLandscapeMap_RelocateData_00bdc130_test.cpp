#include <stdio.h>
#include <string.h>
#include "candidates/landscape_map_open_compiled.h"

int main()
{
    CEngineLandscapeMap map;
    memset(&map, 0, sizeof(map));
    map.RelocateData(7, reinterpret_cast<void*>(0x1000), reinterpret_cast<void*>(0x2000), 0x1000);
    printf("LANDSCAPE_MAP_RELOCATE_DATA_TEST PASS\n");
    return 0;
}

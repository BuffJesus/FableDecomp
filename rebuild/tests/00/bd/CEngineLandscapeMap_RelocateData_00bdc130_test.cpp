#include <stdio.h>
#include <string.h>
extern "C" void __fastcall Relocate(void*,void*,long,void*,void*,long);
#pragma comment(linker,"/alternatename:@Relocate@24=?RelocateData@CEngineLandscapeMap@@EAEXJPAX0J@Z")
int main() {
    unsigned long map[24],before[24];
    memset(map,0x95,sizeof(map));memcpy(before,map,sizeof(map));
    Relocate(map,0,7,(void*)0x1000,(void*)0x2000,0x1000);
    if(memcmp(map,before,sizeof(map)))return 1;
    printf("LANDSCAPE_MAP_RELOCATE_DATA_TEST PASS\n");return 0;
}

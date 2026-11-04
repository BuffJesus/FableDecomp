#include <stdio.h>

class CGameComponent;
class CMemoryAllocator;
class CProfileManager;
extern CGameComponent* g_FableRetiredGameComponent_013B7D58;
extern unsigned long g_FableXMVCodeHandle_013CA7E0;
extern unsigned long g_FableXMVCodeReferences_013CA7E4;
extern CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
extern CProfileManager* g_FableProfileManager_013CB4F8;

int main()
{
    if (g_FableRetiredGameComponent_013B7D58 || g_FableXMVCodeHandle_013CA7E0 ||
        g_FableXMVCodeReferences_013CA7E4 || g_FableMemoryAllocator_013CAA90 ||
        g_FableProfileManager_013CB4F8) return 1;
    unsigned char storage[3][16];
    for (int i = 0; i < 16; ++i) {
        g_FableRetiredGameComponent_013B7D58 = reinterpret_cast<CGameComponent*>(storage[0] + i);
        g_FableXMVCodeHandle_013CA7E0 = 100 + i;
        g_FableXMVCodeReferences_013CA7E4 = 200 + i;
        g_FableMemoryAllocator_013CAA90 = reinterpret_cast<CMemoryAllocator*>(storage[1] + i);
        g_FableProfileManager_013CB4F8 = reinterpret_cast<CProfileManager*>(storage[2] + i);
        if (reinterpret_cast<void*>(g_FableRetiredGameComponent_013B7D58) != storage[0] + i ||
            g_FableXMVCodeHandle_013CA7E0 != static_cast<unsigned long>(100 + i) ||
            g_FableXMVCodeReferences_013CA7E4 != static_cast<unsigned long>(200 + i) ||
            reinterpret_cast<void*>(g_FableMemoryAllocator_013CAA90) != storage[1] + i ||
            reinterpret_cast<void*>(g_FableProfileManager_013CB4F8) != storage[2] + i) return 2;
    }
    puts("GFMAIN_SINGLETON_CODE_DATA_PASS cases=16");
    return 0;
}

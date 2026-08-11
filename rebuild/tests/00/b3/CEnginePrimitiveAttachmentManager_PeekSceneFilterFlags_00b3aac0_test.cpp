#include <cstdio>

struct CEnginePrimitiveAttachmentManager {
    unsigned long __fastcall PeekSceneFilterFlags();
};

unsigned long __fastcall CEnginePrimitiveAttachmentManager::PeekSceneFilterFlags()
{
    return 3;
}

int main()
{
    CEnginePrimitiveAttachmentManager m;
    unsigned long v = m.PeekSceneFilterFlags();
    if (v == 3) {
        printf("00b3aac0_TEST PASS\n");
    } else {
        printf("FAIL %lu\n", v);
    }
    return 0;
}
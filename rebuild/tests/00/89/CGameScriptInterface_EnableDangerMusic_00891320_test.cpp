#include <cstdio>
struct Node { char pad4[4]; char* at4; };
static void* g_seen = 0;
extern "C" void __fastcall EnableDangerMusic_Target_impl(void* self) { g_seen = self; }
void __fastcall CGameScriptInterface_EnableDangerMusic(Node* self)
{
    char* p = *(char**)((char*)self + 4);
    void* q = *(void**)(p + 0x6c);
    EnableDangerMusic_Target_impl(q);
}
int main() {
    char blockC[0x80]; char blockB[0x80]; char blockA[0x80];
    void* target = (void*)blockC;
    *(void**)(blockB + 0x6c) = target;
    *(char**)(blockA + 4) = blockB;
    CGameScriptInterface_EnableDangerMusic((Node*)blockA);
    if (g_seen == target) { std::printf("CGameScriptInterface_00891320_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}
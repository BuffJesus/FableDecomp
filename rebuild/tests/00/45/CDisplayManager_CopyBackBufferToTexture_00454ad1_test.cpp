#include <cstdio>
struct CTexture;
struct CDisplayManager {
    unsigned char _pad_0x0[0x54];
    void*** obj54;
    unsigned int f5c;
    unsigned int f60;
    unsigned int f1c4;
    int field1c4;
};

static int g_calls = 0;
static char* g_a = 0;
static char* g_b = 0;
void DM_Helper(char* a, char* b) { g_calls++; g_a = a; g_b = b; }

CTexture* __fastcall CDisplayManager_CopyBackBufferToTexture(CDisplayManager* self)
{
    char local;
    DM_Helper(&local, &local);
    return (CTexture*)self;
}

int main() {
    CDisplayManager dm;
    CTexture* r = CDisplayManager_CopyBackBufferToTexture(&dm);
    if ((void*)r == (void*)&dm && g_calls == 1 && g_a == g_b && g_a != 0) {
        std::printf("CDisplayManager_00454ad1_TEST PASS\n");
        return 0;
    }
    std::printf("CDisplayManager_00454ad1_TEST FAIL\n");
    return 1;
}
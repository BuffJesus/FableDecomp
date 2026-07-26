#include <cstdio>

// Backing storage; g_bitDepthTable points at record 0's key so that negative
// indices reach the flag (-1) and payload (-2) fields, matching retail's edi.
static int g_backing[64];
int* const g_bitDepthTable = &g_backing[2];

struct CDisplayManager {
    char  pad0[0x54];
    void* dev;                 // +0x54
    char  pad1[0x04];
    int   field5c;             // +0x5c
    int   field60;             // +0x60
    char  pad2[0x1c4 - 0x64];
    int   field1c4;            // +0x1c4
    bool  IsTextureBitDepthSupported(int fmt);
};

static int g_lastArg = -999;
int __stdcall fake_check(void* dev, int a, int b, int c, int d, int e, int f)
{
    (void)dev; (void)a; (void)b; (void)c; (void)d; (void)e;
    g_lastArg = f;
    return (f == 7777) ? 0 : -1;   // supported only for payload 7777
}

static void* g_vt[16];

bool CDisplayManager::IsTextureBitDepthSupported(int fmt)
{
    if (g_bitDepthTable[0] != -1) {
        int* p = g_bitDepthTable;
        do {
            if (p[-1] == 2 && fmt == p[0]) {
                void*  dev = this->dev;
                void** vt  = *(void***)dev;
                int r = ((int(__stdcall*)(void*,int,int,int,int,int,int))vt[10])(
                    dev, this->field60, this->field5c, this->field1c4, 0, 3, p[-2]);
                if (r >= 0)
                    return true;
            }
            p += 9;
        } while (p[0] != -1);
    }
    return false;
}

int main()
{
    g_vt[10] = (void*)&fake_check;
    struct Dev { void* vt; } dev;
    dev.vt = g_vt;

    CDisplayManager dm;
    dm.dev = &dev;
    dm.field5c = 111;
    dm.field60 = 222;
    dm.field1c4 = 333;

    for (int i = 0; i < 64; ++i) g_backing[i] = 0;

    // record0: payload=[-2], flag=[-1], key=[0]. stride 9 dwords.
    g_bitDepthTable[-2] = 7777; g_bitDepthTable[-1] = 2; g_bitDepthTable[0] = 500;
    // record1 (base +9): payload=[7], flag=[8], key=[9]
    g_bitDepthTable[7]  = 42;   g_bitDepthTable[8]  = 2; g_bitDepthTable[9] = 600;
    // record2 key = -1 -> terminator (base +18)
    g_bitDepthTable[18] = -1;

    bool r1 = dm.IsTextureBitDepthSupported(500); // rec0 payload 7777 -> supported
    bool r2 = dm.IsTextureBitDepthSupported(600); // rec1 payload 42 -> not, then term -> false
    bool r3 = dm.IsTextureBitDepthSupported(999); // no match -> false

    // Whole-table-disabled short circuit (key of record0 == -1)
    int savedKey = g_bitDepthTable[0];
    g_bitDepthTable[0] = -1;
    bool r4 = dm.IsTextureBitDepthSupported(500);
    g_bitDepthTable[0] = savedKey;

    if (r1 == true && r2 == false && r3 == false && r4 == false && g_lastArg == 42) {
        std::printf("CDisplayManager_009be530_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r1=%d r2=%d r3=%d r4=%d lastArg=%d\n",
                (int)r1, (int)r2, (int)r3, (int)r4, g_lastArg);
    return 1;
}
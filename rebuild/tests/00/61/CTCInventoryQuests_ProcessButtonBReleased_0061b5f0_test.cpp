#include <cstdio>

struct CTCInventoryQuests {
    char pad[0x157];
    unsigned char m_flag;   // +0x157
};

static int g_a = 0;
static int g_b = 0;

void __fastcall CTCInventoryQuests_HelperA(CTCInventoryQuests* self) { (void)self; g_a++; }
void __fastcall CTCInventoryQuests_HelperB(CTCInventoryQuests* self) { (void)self; g_b++; }

void __fastcall CTCInventoryQuests_ProcessButtonBReleased(CTCInventoryQuests* self);

int main() {
    CTCInventoryQuests o1; o1.m_flag = 0;
    g_a = g_b = 0;
    CTCInventoryQuests_ProcessButtonBReleased(&o1);
    // flag clear: A not called, B called
    bool ok1 = (g_a == 0 && g_b == 1);

    CTCInventoryQuests o2; o2.m_flag = 1;
    g_a = g_b = 0;
    CTCInventoryQuests_ProcessButtonBReleased(&o2);
    // flag set: A called, B called
    bool ok2 = (g_a == 1 && g_b == 1);

    if (ok1 && ok2) { printf("PROCESSB_OK\n"); return 0; }
    printf("FAIL a=%d b=%d\n", g_a, g_b);
    return 1;
}
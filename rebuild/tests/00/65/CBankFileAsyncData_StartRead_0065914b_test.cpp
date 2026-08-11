#include <stdio.h>

struct SubObj {
    int first;
    int Read(int* pLocal);
};

struct CBankFileAsyncData {
    char pad[0x64];
    SubObj sub;
    bool StartRead(int scratch);
};

static int g_helperRet;
static SubObj* g_seenSelf;
static int* g_seenLocal;

int SubObj::Read(int* pLocal)
{
    g_seenSelf = this;
    g_seenLocal = pLocal;
    return g_helperRet;
}

int main()
{
    CBankFileAsyncData obj;
    obj.sub.first = 0x1234;

    g_helperRet = 0x9999;
    bool r1 = obj.StartRead(0);

    g_helperRet = 0x1234;
    bool r2 = obj.StartRead(0);

    bool selfok = ((char*)g_seenSelf == (char*)&obj + 0x64);

    if (r1 == true && r2 == false && selfok) {
        printf("0065914b_TEST PASS\n");
    } else {
        printf("FAIL r1=%d r2=%d selfok=%d\n", (int)r1, (int)r2, (int)selfok);
    }
    return 0;
}
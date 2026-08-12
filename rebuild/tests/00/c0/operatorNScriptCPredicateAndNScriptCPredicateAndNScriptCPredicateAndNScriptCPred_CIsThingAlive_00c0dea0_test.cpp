#include <stdio.h>

struct CIsThingAlive;

struct CPredicate_And {
    void helper(CIsThingAlive* arg);
};

CPredicate_And* __fastcall operator_And(CPredicate_And* self, CIsThingAlive* arg);

static void* g_lastSelf;
static void* g_lastArg;
void CPredicate_And::helper(CIsThingAlive* arg) {
    g_lastSelf = this;
    g_lastArg = arg;
}

int main() {
    char selfBuf[8];
    char argBuf[8];
    CPredicate_And* self = (CPredicate_And*)selfBuf;
    CIsThingAlive* arg = (CIsThingAlive*)argBuf;

    CPredicate_And* ret = operator_And(self, arg);

    bool ok = (ret == self) && (g_lastSelf == (void*)self) && (g_lastArg == (void*)arg);
    printf("ret=%p self=%p lastSelf=%p lastArg=%p\n", (void*)ret, (void*)self, g_lastSelf, g_lastArg);
    if (ok) {
        printf("PASS_OPERATOR_AND_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}
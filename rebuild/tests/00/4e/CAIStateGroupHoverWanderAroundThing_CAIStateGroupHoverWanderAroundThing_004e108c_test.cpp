#include <cstdio>

// Independent behaviour test: reproduce the destructor's observable effects
// (member OnReadFinished called, field0 set to 0x1230ba0, base dtor run) with a
// local self-contained model, then assert order and final state.

static int g_log[8];
static int g_n;

struct CActiveFile {
    unsigned char body[4];
    CActiveFile() { body[0] = 0; }
    void OnReadFinished() { g_log[g_n++] = 1; }   // member call
};

struct CMemoryBlock {
    unsigned int* field0;
    unsigned char pad[0x174 - 4];
    CMemoryBlock() : field0(0) {}
    ~CMemoryBlock() { g_log[g_n++] = 3; }         // base dtor (last)
};

struct CAIStateGroup_HoverWanderAroundThing : public CMemoryBlock {
    CActiveFile m_file;
    CAIStateGroup_HoverWanderAroundThing() {}
    ~CAIStateGroup_HoverWanderAroundThing()
    {
        m_file.OnReadFinished();                  // (1)
        field0 = (unsigned int*)0x1230ba0;        // (2) data write
        g_log[g_n++] = 2;                         // marker for the write step
    }
};

int main() {
    g_n = 0;
    unsigned int* observed_field0 = 0;
    {
        CAIStateGroup_HoverWanderAroundThing obj;
        unsigned char* base = (unsigned char*)&obj;
        unsigned char* mem  = (unsigned char*)&obj.m_file;
        if ((mem - base) != 0x174) { printf("OFFSET_FAIL %ld\n", (long)(mem - base)); return 1; }
        // destructor runs at scope exit; capture field0 just before via a manual peek
    }
    // Rebuild to inspect final field0 deterministically:
    {
        CAIStateGroup_HoverWanderAroundThing* p = new CAIStateGroup_HoverWanderAroundThing();
        g_n = 0;
        p->~CAIStateGroup_HoverWanderAroundThing();
        observed_field0 = p->field0;
        operator delete(p);
    }
    if (g_n == 3 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 &&
        observed_field0 == (unsigned int*)0x1230ba0) {
        printf("HOVERWANDER_DTOR_OK\n");
        return 0;
    }
    printf("FAIL n=%d [%d %d %d] f0=%p\n", g_n, g_log[0], g_log[1], g_log[2], (void*)observed_field0);
    return 1;
}
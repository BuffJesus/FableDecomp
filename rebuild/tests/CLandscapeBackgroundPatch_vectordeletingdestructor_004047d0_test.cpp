#include <cstdio>

static int g_calls[8];
static void* g_deleted;

struct CInner { void* v; };

void Inner14_dtor(void* p){ g_calls[0]++; }
void Inner_dtor(void* p){ g_calls[1]++; }
void CLBP_dtor(void* p){ g_calls[2]++; }

struct CLandscapeBackgroundPatch {
    void* vftable;
    CInner inner4;
    void* f8;
    CInner innerC;
    unsigned char _pad_0x10[0xa8];
    void* tex;
};

static void mydelete(void* p){ g_deleted = p; }

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    Inner14_dtor((char*)self + 0x14);
    Inner_dtor((char*)self + 0x10);
    Inner_dtor((char*)self + 0xc);
    CLBP_dtor(self);
    if (flags & 1)
        mydelete(self);
    return self;
}

int main(){
    static unsigned char buf[0x100];
    CLandscapeBackgroundPatch* p = (CLandscapeBackgroundPatch*)buf;
    // flags without delete bit
    for(int i=0;i<8;i++) g_calls[i]=0; g_deleted=0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 0);
    if (r != p) { std::printf("FAIL ret\n"); return 1; }
    if (g_calls[0]!=1 || g_calls[1]!=2 || g_calls[2]!=1) { std::printf("FAIL dtor counts\n"); return 1; }
    if (g_deleted != 0) { std::printf("FAIL unexpected delete\n"); return 1; }
    // flags with delete bit
    for(int i=0;i<8;i++) g_calls[i]=0; g_deleted=0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(p, 0, 1);
    if (r != p) { std::printf("FAIL ret2\n"); return 1; }
    if (g_deleted != p) { std::printf("FAIL no delete\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004047d0_TEST PASS\n");
    return 0;
}
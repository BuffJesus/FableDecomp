#include <cstdio>

struct CWideString { int v; };

static int g_r;
static void* g_ecx; static void* g_edx; static int g_arg;
static int g_tail_called;
static CWideString* g_tp2; static long g_tp3; static bool g_tp4;

int __fastcall callee_5d97b0(CWideString* self){ return self->v + 100; }

struct Tail {
    void tail_5bf19c(CWideString* p2, long p3, bool p4){
        g_tail_called = 1; g_tp2 = p2; g_tp3 = p3; g_tp4 = p4;
    }
};
static Tail g_tail;

extern "C" Tail* __fastcall callee_5c08c0(void* ecx, void* edx, int arg){
    g_ecx = ecx; g_edx = edx; g_arg = arg; return &g_tail;
}

struct CTCInventoryBase {
    char pad[0x20];
    void* m20;
    void* m24;
    void GetItemDescription(CWideString* p2, long p3, bool p4);
};

void CTCInventoryBase::GetItemDescription(CWideString* p2, long p3, bool p4)
{
    int r = callee_5d97b0(p2);
    Tail* t = (Tail*)callee_5c08c0(this->m20, this->m24, r);
    t->tail_5bf19c(p2, p3, p4);
}

int main(){
    CTCInventoryBase inv;
    inv.m20 = (void*)0x1111; inv.m24 = (void*)0x2222;
    CWideString ws; ws.v = 5;
    g_tail_called = 0;
    inv.GetItemDescription(&ws, 7, true);
    if(g_arg==105 && g_ecx==(void*)0x1111 && g_edx==(void*)0x2222 &&
       g_tail_called==1 && g_tp2==&ws && g_tp3==7 && g_tp4==true)
        printf("PARITY_OK_005bf2ed\n");
    else
        printf("FAIL\n");
    return 0;
}
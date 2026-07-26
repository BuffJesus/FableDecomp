#include <cstdio>
static void* g_calls[8];
static int g_n = 0;
struct CThingMarkerDef;
struct CXfer {
    void M70(void* p){ g_calls[g_n++] = p; }
    void M80(void* p){ g_calls[g_n++] = p; }
    void M84(void* p){ g_calls[g_n++] = p; }
};
struct CThingMarkerDef {
    char b[0x100];
    void BaseTransfer(CXfer* x){ g_calls[g_n++] = this; g_calls[g_n++] = x; }
    void Transfer(CXfer* x);
};
void CThingMarkerDef::Transfer(CXfer* x)
{
    BaseTransfer(x);
    x->M70((char*)this + 0x70);
    x->M80((char*)this + 0x80);
    x->M84((char*)this + 0x84);
}
int main(){
    CThingMarkerDef d;
    CXfer x;
    d.Transfer(&x);
    if (g_calls[0]==&d && g_calls[1]==&x &&
        g_calls[2]==(char*)&d+0x70 &&
        g_calls[3]==(char*)&d+0x80 &&
        g_calls[4]==(char*)&d+0x84){
        std::printf("CThingMarkerDef_00454e21_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
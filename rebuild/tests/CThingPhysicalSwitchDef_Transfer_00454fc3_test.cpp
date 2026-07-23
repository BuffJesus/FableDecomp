#include <cstdio>

static int g_seq;
static int g_log[8];
static int g_baseCalled;

struct CSerial {
    void mA(void* p);
    void mB(void* p);
    void mC(void* p);
};

struct CThingPhysicalSwitchDef {
    void BaseTransfer(CSerial* s);
};

void CSerial::mA(void* p){ g_log[g_seq++] = 1; }
void CSerial::mB(void* p){ g_log[g_seq++] = 2; }
void CSerial::mC(void* p){ g_log[g_seq++] = 3; }
void CThingPhysicalSwitchDef::BaseTransfer(CSerial* s){ g_baseCalled = 1; }

void __fastcall CThingPhysicalSwitchDef_Transfer(CThingPhysicalSwitchDef* self, void* edx, CSerial* s)
{
    self->BaseTransfer(s);
    char* base = (char*)self;
    s->mA(base + 0x70);
    s->mB(base + 0x74);
    s->mB(base + 0x82);
    s->mC(base + 0x90);
}

int main(){
    char buf[0x100];
    CThingPhysicalSwitchDef* obj = (CThingPhysicalSwitchDef*)buf;
    CSerial s;
    g_seq = 0; g_baseCalled = 0;
    for(int i=0;i<8;i++) g_log[i]=0;
    CThingPhysicalSwitchDef_Transfer(obj, 0, &s);
    if(g_baseCalled==1 && g_log[0]==1 && g_log[1]==2 && g_log[2]==2 && g_log[3]==3 && g_seq==4){
        std::printf("CThingPhysicalSwitchDef_00454fc3_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL base=%d seq=%d\n", g_baseCalled, g_seq);
    return 1;
}
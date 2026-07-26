#include <cstdio>
struct SubA { char x; };
struct SubB { char x; };
#pragma pack(1)
struct CTCInventoryItem {
    unsigned char _pad_0x0[0x3c];
    SubB sub3c;
    unsigned char _pad_0x3d[0x13];
    SubA sub50;
    unsigned char _pad_0x51[0x1b];
    void* ptr6c;
};
#pragma pack()
static int g_free=0, g_a=0, g_b=0, g_base=0;
extern "C" void __cdecl engine_free(void* p){ (void)p; g_free++; }
void __fastcall SubA_dtor(SubA* self){ (void)self; g_a++; }
void __fastcall SubB_dtor(SubB* self){ (void)self; g_b++; }
void __fastcall CTCInventoryItem_base(CTCInventoryItem* self){ (void)self; g_base++; }

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr6c)
        engine_free(self->ptr6c);
    SubA_dtor(&self->sub50);
    SubB_dtor(&self->sub3c);
    CTCInventoryItem_base(self);
}

int main(){
    CTCInventoryItem it; 
    for (int i=0;i<(int)sizeof(it);++i) ((unsigned char*)&it)[i]=0;
    int dummy=5; it.ptr6c=&dummy;
    CTCInventoryItem_OnDie(&it);
    if (g_free!=1||g_a!=1||g_b!=1||g_base!=1){ std::printf("FAIL path1\n"); return 1; }
    it.ptr6c=0; g_free=g_a=g_b=g_base=0;
    CTCInventoryItem_OnDie(&it);
    if (g_free!=0||g_a!=1||g_b!=1||g_base!=1){ std::printf("FAIL path2\n"); return 1; }
    std::printf("CTCInventoryItem_0045704a_TEST PASS\n");
    return 0;
}
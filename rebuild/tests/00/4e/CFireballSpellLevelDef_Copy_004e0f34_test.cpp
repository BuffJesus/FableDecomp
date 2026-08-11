#include <cstdio>
struct CDefClassBase;
struct CFireballSpellLevelDef {
    char pad[0x28];
    unsigned long field28;
    void InheritFrom(CDefClassBase const* src);
    void Copy(CDefClassBase const* src);
};
static void* g_self=0; static const void* g_src=0;
void CFireballSpellLevelDef::InheritFrom(CDefClassBase const* src){ g_self=this; g_src=src; }
int main(){
    CFireballSpellLevelDef dst; dst.field28=0;
    CFireballSpellLevelDef srcObj; srcObj.field28=0xDEADBEEF;
    dst.Copy((CDefClassBase const*)&srcObj);
    if(dst.field28==0xDEADBEEF && g_self==&dst && g_src==(void*)&srcObj) printf("004e0f34_TEST PASS\n");
    else printf("FAIL %lx\n",dst.field28);
    return 0;
}
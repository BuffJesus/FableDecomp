#include <stdio.h>
typedef struct MsgEventList { void* m0; } MsgEventList;
static int g_subCalls = 0;
static void* g_freed = 0;
static void SubDtor(MsgEventList* self){ (void)self; g_subCalls++; }
static void myfree(void* p){ g_freed = p; }
static void DtorModel(MsgEventList* self)
{
    SubDtor(self);
    myfree(self->m0);
}
int main(){
    int payload = 0;
    void* ppayload = &payload + 0;
    MsgEventList e; e.m0 = ppayload;
    DtorModel(&e + 0);
    if (g_subCalls==1 && g_freed==ppayload){ printf("OK_00c0c910\n"); return 0; }
    printf("FAIL\n"); return 1;
}

#include <cstdio>
struct MsgEventList { void* p0; };
static int g_sub=0, g_free=0; static void* g_freed=(void*)0;
void __fastcall MsgEventList_SubDtor(MsgEventList* self){ g_sub++; }
static void __cdecl myfree(void* p){ g_free++; g_freed=p; }
typedef void (__cdecl *FreeFn)(void*);
static FreeFn slot = myfree;
void __fastcall DtorMsgEventList_B(MsgEventList* self){
    MsgEventList_SubDtor(self);
    slot(self->p0);
}
int main(){
    int payload=0x1234; MsgEventList m; m.p0=&payload;
    DtorMsgEventList_B(&m);
    if(g_sub==1 && g_free==1 && g_freed==&payload) printf("OK_00c0c930\n");
    else { printf("FAIL sub=%d free=%d\n",g_sub,g_free); return 1; }
    return 0;
}
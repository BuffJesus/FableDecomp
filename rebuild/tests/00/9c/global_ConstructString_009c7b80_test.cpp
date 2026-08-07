#include "rebuild_abi.h"
#include <cstdio>
static int g_len_9c7b80=0; static void *g_self_9c7b80=0, *g_arg_9c7b80=0;
extern "C" void FABLE_CDECL FableStringCtor_9c7b80(void *self, void *edxv, void *arg, int len){ g_self_9c7b80=self; g_arg_9c7b80=arg; g_len_9c7b80=len; }
extern "C" void FABLE_FASTCALL FableConstructString_9c7b80(void *self, void *edxv, void *arg){ FableStringCtor_9c7b80(self, edxv, arg, -1); }
int main(){
    FableConstructString_9c7b80((void*)1,(void*)2,(void*)3);
    if(g_self_9c7b80!=(void*)1||g_arg_9c7b80!=(void*)3||g_len_9c7b80!=-1){std::printf("BAD\n");return 1;}
    std::printf("CONSTRSTR_OK\n"); return 0;
}
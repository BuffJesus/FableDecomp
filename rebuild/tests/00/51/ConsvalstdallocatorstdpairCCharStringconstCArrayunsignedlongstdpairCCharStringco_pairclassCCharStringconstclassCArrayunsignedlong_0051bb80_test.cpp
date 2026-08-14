#include <cstdio>
#include <cstring>

static int g_char_calls = 0;
static int g_arr_calls = 0;
static void* g_char_self = 0;
static void* g_char_src = 0;
static void* g_arr_self = 0;
static void* g_arr_src = 0;

struct CCharString {
    char pad[0x0c];
    void CopyFrom(void* src){ g_char_calls++; g_char_self=this; g_char_src=src; }
};
struct CArrayUL {
    char pad[0x0c];
    void CopyFrom(void* src){ g_arr_calls++; g_arr_self=this; g_arr_src=src; }
};
struct PairKV {
    CCharString first;
    CArrayUL    second;
};

void __fastcall Cons_val_pair(PairKV* self, PairKV* src)
{
    if (self) {
        self->first.CopyFrom(&src->first);
        self->second.CopyFrom(&src->second);
    }
}

int main(){
    PairKV dst, src;
    memset(&dst,0,sizeof(dst)); memset(&src,0,sizeof(src));
    Cons_val_pair(0, &src);
    if (g_char_calls!=0 || g_arr_calls!=0){ printf("BAD1\n"); return 1; }
    Cons_val_pair(&dst, &src);
    if (g_char_calls!=1 || g_arr_calls!=1){ printf("BAD2\n"); return 1; }
    if (g_char_self!=(void*)&dst || g_char_src!=(void*)&src){ printf("BAD3\n"); return 1; }
    if (g_arr_self!=(void*)((char*)&dst+0x0c) || g_arr_src!=(void*)((char*)&src+0x0c)){ printf("BAD4\n"); return 1; }
    printf("CONSVAL_OK\n");
    return 0;
}
#include <cstdio>
struct Sub { int q; };
struct M { char pad[0xa8]; Sub sub; };
static int g_helper=0; static int g_final=0; static Sub* g_hs=0; static M* g_fs=0;
extern "C" void __fastcall helper_sub(Sub* s){ g_helper++; g_hs=s; }
extern "C" void __fastcall final_music(M* self){ g_final++; g_fs=self; }
void __fastcall DestVal_Music_A(M* self, void* /*edx*/){ helper_sub(&self->sub); final_music(self); }
int main(){
    M m; 
    DestVal_Music_A(&m,0);
    if(g_helper!=1||g_final!=1){ printf("FAIL counts\n"); return 1; }
    if((char*)g_hs != (char*)&m + 0xa8){ printf("FAIL suboffset\n"); return 1; }
    if(g_fs != &m){ printf("FAIL self\n"); return 1; }
    printf("OK_00628f10\n");
    return 0;
}
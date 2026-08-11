#include <cstdio>

struct CTCLook;

static void* g_self; static char g_b; static void* g_p1; static void* g_p2;
extern bool __fastcall Helper_00c0aee0(void* self, char b, void* p1, void* p2);
bool __fastcall Helper_00c0aee0(void* self, char b, void* p1, void* p2){
    g_self=self; g_b=b; g_p1=p1; g_p2=p2; return true;
}

bool __fastcall IsTurningBody(CTCLook* self, int edx, void* A, char B);

int main(){
    char buf[0x100];
    CTCLook* self=(CTCLook*)buf;
    void* A=(void*)0x11110000;
    char B=(char)0x55;
    bool r=IsTurningBody(self,0,A,B);
    bool ok = r && g_self==A && g_b==(char)0x55 && g_p1==(buf+0x4c) && g_p2==(buf+0x2a);
    if(ok) printf("00c0a8a0_TEST PASS\n"); else printf("FAIL r=%d b=%d\n",r,(int)g_b);
    return 0;
}
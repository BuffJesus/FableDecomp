
#include <cstdio>
struct CActiveFile { void* p0; };
static int g_helper=0; static void* g_freed=(void*)0;
void model(CActiveFile* self){
    g_helper++;
    void* p=self->p0;
    if(p) g_freed=p;
}
int main(){
    CActiveFile a; a.p0=(void*)0x1234;
    model(&a);
    if(g_helper!=1||g_freed!=(void*)0x1234){printf("FAIL1\n");return 1;}
    CActiveFile b; b.p0=(void*)0; g_freed=(void*)0;
    model(&b);
    if(g_helper!=2||g_freed!=(void*)0){printf("FAIL2\n");return 1;}
    printf("OK_004dd1db\n");
    return 0;
}
#include <stdio.h>
struct CActiveFile { void* p0; };
static int g_helper_called = 0;
static int g_free_called = 0;
static void* g_freed = 0;
void helper(CActiveFile* self){ g_helper_called++; }
void myfree(void* p){ g_free_called++; g_freed = p; }
void model(CActiveFile* self){
    helper(self);
    void* p = self->p0;
    if (p) myfree(p);
}
int main(){
    int dummy;
    CActiveFile a; a.p0 = &dummy;
    model(&a);
    if (g_helper_called!=1) { printf("FAIL helper\n"); return 1; }
    if (g_free_called!=1 || g_freed!=&dummy) { printf("FAIL free\n"); return 1; }
    CActiveFile b; b.p0 = 0;
    g_helper_called=0; g_free_called=0;
    model(&b);
    if (g_helper_called!=1) { printf("FAIL helper2\n"); return 1; }
    if (g_free_called!=0) { printf("FAIL free2\n"); return 1; }
    printf("OK_004dd159\n");
    return 0;
}
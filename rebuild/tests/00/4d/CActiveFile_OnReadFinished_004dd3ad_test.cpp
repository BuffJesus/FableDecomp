#include <cstdio>
struct CActiveFile { void* m_ptr; };
static int g_helper_called;
static void* g_freed;
static void helper(CActiveFile* self){ g_helper_called++; }
static void freefn(void* p){ g_freed = p; }
static void model(CActiveFile* self){
    helper(self);
    void* p = self->m_ptr;
    if (p) freefn(p);
}
int main(){
    // non-null branch
    g_helper_called=0; g_freed=0;
    CActiveFile a; int dummy; a.m_ptr=&dummy;
    model(&a);
    if (g_helper_called!=1) { printf("FAIL helper\n"); return 1; }
    if (g_freed!=&dummy) { printf("FAIL free\n"); return 1; }
    // null branch
    g_helper_called=0; g_freed=(void*)1;
    CActiveFile b; b.m_ptr=0;
    model(&b);
    if (g_helper_called!=1) { printf("FAIL helper2\n"); return 1; }
    if (g_freed!=(void*)1) { printf("FAIL freed2\n"); return 1; }
    printf("OK_004dd3ad\n");
    return 0;
}
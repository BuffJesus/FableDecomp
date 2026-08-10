#include <stdio.h>
struct CActiveFile { void* field0; };
static int g_helper_called = 0;
static int g_free_called = 0;
static void* g_freed = 0;
void AF_helper(CActiveFile* self){ g_helper_called++; }
void AF_free(void* p){ g_free_called++; g_freed = p; }
void OnReadFinished_12(CActiveFile* self)
{
    AF_helper(self);
    void* p = self->field0;
    if (p)
        AF_free(p);
}
int main(){
    int dummy = 5;
    CActiveFile a; a.field0 = &dummy;
    OnReadFinished_12(&a);
    if (g_helper_called != 1) { printf("FAIL helper\n"); return 1; }
    if (g_free_called != 1) { printf("FAIL free\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL ptr\n"); return 1; }
    CActiveFile b; b.field0 = 0;
    g_helper_called = 0; g_free_called = 0;
    OnReadFinished_12(&b);
    if (g_helper_called != 1) { printf("FAIL helper2\n"); return 1; }
    if (g_free_called != 0) { printf("FAIL free2\n"); return 1; }
    printf("OK_004dd52b\n");
    return 0;
}
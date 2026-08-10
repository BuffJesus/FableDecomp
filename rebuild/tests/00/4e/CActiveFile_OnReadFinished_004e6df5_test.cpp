#include <stdio.h>
#include <stdlib.h>
struct CActiveFile { void* buf; };
static int g_helper_called = 0;
static void helper_read(CActiveFile* self){ g_helper_called++; (void)self; }
static int g_freed = 0;
static void my_free(void* p){ if(p) g_freed++; }
static void OnReadFinished_3_model(CActiveFile* self){
    helper_read(self);
    void* p = self->buf;
    if(p) my_free(p);
}
int main(){
    CActiveFile a; a.buf = malloc(8);
    OnReadFinished_3_model(&a);
    CActiveFile b; b.buf = 0;
    OnReadFinished_3_model(&b);
    if(g_helper_called==2 && g_freed==1){ printf("OK_004e6df5\n"); return 0; }
    printf("FAIL\n"); return 1;
}
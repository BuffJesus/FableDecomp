#include <stdio.h>
struct CActiveFile { void* m0; };
static int cleanup_calls = 0;
static int free_calls = 0;
static void* freed_ptr = 0;
void CAF_cleanup(CActiveFile* self){ cleanup_calls++; }
void CAF_free(void* p){ free_calls++; freed_ptr = p; }
void OnReadFinished_7(CActiveFile* self){
    CAF_cleanup(self);
    void* p = self->m0;
    if (p) CAF_free(p);
}
int main(){
    int ok = 1;
    int dummy = 42;
    CActiveFile a; a.m0 = &dummy;
    cleanup_calls=free_calls=0; freed_ptr=0;
    OnReadFinished_7(&a);
    if(cleanup_calls!=1) ok=0;
    if(free_calls!=1) ok=0;
    if(freed_ptr!=&dummy) ok=0;
    CActiveFile b; b.m0 = 0;
    cleanup_calls=free_calls=0; freed_ptr=0;
    OnReadFinished_7(&b);
    if(cleanup_calls!=1) ok=0;
    if(free_calls!=0) ok=0;
    if(ok) printf("OK_004dd36c\n");
    else printf("FAIL\n");
    return ok?0:1;
}
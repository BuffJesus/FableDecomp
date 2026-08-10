
#include <stdio.h>
struct T { void* v; void* f4; void* f8; void* fc; };
struct VT { void* s0; void* s1; void* s2; void* s3; T* (__fastcall* clone)(void*); };
static T g_new;
static T* __fastcall do_clone(void* self){ g_new.f8=(void*)1; g_new.fc=(void*)2; return &g_new; }
static VT g_vt = {0,0,0,0,do_clone};
struct T2 { void* v; void* f4; void* f8; void* fc; };
extern "C" T* clone_model(T* self){
    VT* vt=*(VT**)self; T* n=vt->clone(self); n->fc=self->fc; n->f8=self->f8; return n;
}
int main(){
    T self; self.v=&g_vt; self.f4=(void*)0xAA; self.f8=(void*)0x88; self.fc=(void*)0xCC;
    T* r=clone_model(&self);
    if(r==&g_new && r->f8==(void*)0x88 && r->fc==(void*)0xCC){ printf("OK_005db2c0\n"); return 0; }
    printf("FAIL\n"); return 1;
}
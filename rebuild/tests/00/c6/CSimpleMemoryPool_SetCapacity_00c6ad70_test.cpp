
#include <stdio.h>
struct CSimpleMemoryPool { void* vtbl; char pad[0x21c-4]; unsigned long capacity; };
static void* dummy_vtbl;
static CSimpleMemoryPool* SetCapacity_model(CSimpleMemoryPool* self){
    self->vtbl=&dummy_vtbl;
    self->capacity=(((unsigned long)self)+0x1b)&0xfffffff0;
    return self;
}
int main(){
    CSimpleMemoryPool p; p.vtbl=0; p.capacity=0;
    CSimpleMemoryPool* r=SetCapacity_model(&p);
    unsigned long expect=(((unsigned long)&p)+0x1b)&0xfffffff0;
    if(r!=&p){printf("FAIL ret\n");return 1;}
    if(p.vtbl!=&dummy_vtbl){printf("FAIL vtbl\n");return 1;}
    if(p.capacity!=expect){printf("FAIL cap\n");return 1;}
    printf("OK_00c6ad70\n");
    return 0;
}
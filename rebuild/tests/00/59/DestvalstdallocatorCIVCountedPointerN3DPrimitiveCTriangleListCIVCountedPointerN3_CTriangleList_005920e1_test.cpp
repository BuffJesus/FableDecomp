#include <cstdio>
struct RefCounted { void** vtable; int refCount; };
struct CP { RefCounted* ptr; };
static int g_dtor_called = 0;
void __fastcall fake_deleting_dtor(RefCounted* rc){ g_dtor_called++; (void)rc; }
void __fastcall Dest_val_CIVCountedPointer_CTriangleList___(CP* self){
    RefCounted* rc = self->ptr;
    if (rc != 0){
        if (--rc->refCount == 0){ typedef void (__fastcall *pfn)(RefCounted*); (*(pfn*)((char*)rc->vtable + 4))(rc); }
        self->ptr = 0;
    }
}
int main(){
    void* vt[2]; vt[0]=0; vt[1]=(void*)&fake_deleting_dtor;
    CP a; a.ptr=0; Dest_val_CIVCountedPointer_CTriangleList___(&a);
    if (a.ptr!=0){printf("FAIL null\n");return 1;} if(g_dtor_called!=0){printf("FAIL null dtor\n");return 1;}
    RefCounted rc2; rc2.vtable=(void**)vt; rc2.refCount=2; CP b; b.ptr=&rc2;
    Dest_val_CIVCountedPointer_CTriangleList___(&b);
    if(rc2.refCount!=1){printf("FAIL rc2 dec\n");return 1;} if(g_dtor_called!=0){printf("FAIL rc2 dtor\n");return 1;} if(b.ptr!=0){printf("FAIL rc2 clear\n");return 1;}
    RefCounted rc3; rc3.vtable=(void**)vt; rc3.refCount=1; CP c; c.ptr=&rc3;
    Dest_val_CIVCountedPointer_CTriangleList___(&c);
    if(rc3.refCount!=0){printf("FAIL rc3 dec\n");return 1;} if(g_dtor_called!=1){printf("FAIL rc3 dtor\n");return 1;} if(c.ptr!=0){printf("FAIL rc3 clear\n");return 1;}
    printf("PARITY_OK_005920e1\n"); return 0;
}
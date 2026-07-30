#include <cstdio>
int g_OnEngineSurveyLMBHeld_vtbl;
struct Payload { unsigned int a; unsigned int b; };
struct Obj { void* vtbl; void* owner; unsigned int a; unsigned int b; };
struct Obj* __fastcall OnEngineSurveyLMBHeld(struct Obj* self, void* /*edx*/, void* owner, struct Payload* src)
{
    self->owner = owner;
    self->vtbl  = &g_OnEngineSurveyLMBHeld_vtbl;
    self->a = src->a;
    self->b = src->b;
    return self;
}
int main(){
    Obj o; o.vtbl=0; o.owner=0; o.a=0; o.b=0;
    Payload p; p.a=0xAABBCCDD; p.b=0x11223344;
    void* owner=(void*)0xDEADBEEF;
    Payload* psrc=&p;
    Obj* self=&o;
    Obj* ret=0;
    // retail ABI: this=ecx, [esp+4]=owner, [esp+8]=src, ret 8
    __asm {
        mov eax, psrc
        push eax
        mov eax, owner
        push eax
        mov ecx, self
        call OnEngineSurveyLMBHeld
        mov ret, eax
    }
    if(ret!=&o){ printf("BAD ret\n"); return 1; }
    if(o.vtbl!=(void*)&g_OnEngineSurveyLMBHeld_vtbl){ printf("BAD vtbl\n"); return 1; }
    if(o.owner!=(void*)0xDEADBEEF){ printf("BAD owner\n"); return 1; }
    if(o.a!=0xAABBCCDD){ printf("BAD a\n"); return 1; }
    if(o.b!=0x11223344){ printf("BAD b\n"); return 1; }
    printf("OK_0x00447a40\n");
    return 0;
}
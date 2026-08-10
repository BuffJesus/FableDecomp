#include <cstdio>
struct CCreatureActionBase { void* pad0; long* buf; long count; };
__declspec(naked) long __fastcall GetFramesRemaining(CCreatureActionBase*, int, long)
{
    __asm {
        mov eax, dword ptr [ecx+8]
        mov edx, dword ptr [ecx+4]
        push esi
        mov esi, dword ptr [esp+8]
        mov dword ptr [edx+eax*4], esi
        inc dword ptr [ecx+8]
        pop esi
        ret 4
    }
}
int main(){
    long storage[8]={0};
    CCreatureActionBase a; a.pad0=0; a.buf=storage; a.count=0;
    // returns OLD count, stores val at buf[oldcount], increments count
    long r = GetFramesRemaining(&a,0,111);
    if(r!=0){printf("FAIL r0 got %ld\n",r);return 1;}
    if(a.count!=1){printf("FAIL c1\n");return 1;}
    if(storage[0]!=111){printf("FAIL s0\n");return 1;}
    long r2 = GetFramesRemaining(&a,0,222);
    if(r2!=1){printf("FAIL r1 got %ld\n",r2);return 1;}
    if(a.count!=2){printf("FAIL c2\n");return 1;}
    if(storage[1]!=222){printf("FAIL s1\n");return 1;}
    printf("OK_00c61be0\n");
    return 0;
}
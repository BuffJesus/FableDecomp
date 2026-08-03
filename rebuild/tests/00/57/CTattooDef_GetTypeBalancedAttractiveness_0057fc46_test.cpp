#include <stdio.h>
#include <string.h>

// ---- function under test (identical to source_cpp) ----
__declspec(naked) float GetTypeBalancedAttractiveness()
{
    __asm {
        mov eax, dword ptr [esp+4]
        push esi
        mov esi, dword ptr [eax]
        mov eax, dword ptr [eax+4]
        push edi
        sub eax, esi
        push 0x48
        cdq
        pop edi
        idiv edi
        cmp eax, 5
        jne notfive
        fld dword ptr [esi+0x15c]
        fmul dword ptr [ecx+0x54]
        jmp done
    notfive:
        fld dword ptr [ecx+0x54]
    done:
        pop edi
        pop esi
        ret 4
    }
}

// vector-like {begin,end}
struct Vec { char* begin; char* end; };

// helper: invoke with this=ecx, one stack arg (p), ret 4 cleans stack
static float call_it(void* thisp, Vec* p)
{
    float out;
    void* pp = p;
    void* tp = thisp;
    __asm {
        mov ecx, tp
        push pp
        call GetTypeBalancedAttractiveness
        fstp out
    }
    return out;
}

int main()
{
    const int ELEM = 0x48;

    // this object: float at +0x54
    char thisbuf[0x60];
    memset(thisbuf, 0, sizeof(thisbuf));
    float base = 3.0f;
    *(float*)(thisbuf + 0x54) = base;

    // ---- count == 5 branch ----
    // need begin[0x15c] readable -> allocate enough room past begin.
    char* backing = new char[0x200 + ELEM * 5];
    memset(backing, 0, 0x200 + ELEM * 5);
    float factor = 2.5f;
    *(float*)(backing + 0x15c) = factor;

    Vec v5;
    v5.begin = backing;
    v5.end   = backing + ELEM * 5;   // exactly 5 elements

    float r5 = call_it(thisbuf, &v5);
    float exp5 = factor * base;      // 7.5
    if (r5 != exp5) { printf("FAIL five: got %f exp %f\n", r5, exp5); return 1; }

    // ---- count != 5 branch (e.g. 3) ----
    Vec v3;
    v3.begin = backing;
    v3.end   = backing + ELEM * 3;
    float r3 = call_it(thisbuf, &v3);
    if (r3 != base) { printf("FAIL notfive: got %f exp %f\n", r3, base); return 1; }

    // ---- count 0 branch ----
    Vec v0;
    v0.begin = backing;
    v0.end   = backing;
    float r0 = call_it(thisbuf, &v0);
    if (r0 != base) { printf("FAIL zero: got %f exp %f\n", r0, base); return 1; }

    printf("OK_0x0057fc46 five=%f notfive=%f zero=%f\n", r5, r3, r0);
    delete[] backing;
    return 0;
}
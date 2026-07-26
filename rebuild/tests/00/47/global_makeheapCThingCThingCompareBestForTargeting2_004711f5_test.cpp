#include <cstdio>
#include "rebuild_abi.h"

struct CThingHeapElem { char _[8]; };

extern void FABLE_FASTCALL heap_build_44(CThingHeapElem*, CThingHeapElem*, void*);
extern void FABLE_FASTCALL heap_adjust_45(CThingHeapElem*, CThingHeapElem*, int, void*);
void FABLE_FASTCALL make_heap_004711f5(CThingHeapElem*, CThingHeapElem*, void*);

static int g_build_calls = 0;
static int g_adjust_calls = 0;
static CThingHeapElem* g_last_build_first = 0;
static CThingHeapElem* g_last_build_last  = 0;

void FABLE_FASTCALL heap_build_44(CThingHeapElem* f, CThingHeapElem* l, void*) {
    g_build_calls++; g_last_build_first = f; g_last_build_last = l;
}
void FABLE_FASTCALL heap_adjust_45(CThingHeapElem*, CThingHeapElem*, int, void*) {
    g_adjust_calls++;
}

int main() {
    CThingHeapElem buf[64];  // 512 bytes

    // Small range: 16 elements = 128 bytes -> rounded==0x80, NOT > 0x80 -> else path
    make_heap_004711f5(&buf[0], &buf[16], (void*)0x1234);
    int small_ok = (g_build_calls == 1 && g_adjust_calls == 0 &&
                    g_last_build_first == &buf[0] && g_last_build_last == &buf[16]);

    // Large range: 17 elements = 136 bytes rounded -> 0x88 > 0x80 -> split path
    g_build_calls = 0; g_adjust_calls = 0;
    make_heap_004711f5(&buf[0], &buf[17], (void*)0x1234);
    int big_ok = (g_build_calls == 1 && g_adjust_calls == 1 &&
                  g_last_build_first == &buf[0] &&
                  g_last_build_last == (CThingHeapElem*)((char*)&buf[0] + 0x80));

    if (small_ok && big_ok) {
        printf("PASS_004711f5_OK\n");
    } else {
        printf("FAIL small=%d big=%d\n", small_ok, big_ok);
    }
    return 0;
}
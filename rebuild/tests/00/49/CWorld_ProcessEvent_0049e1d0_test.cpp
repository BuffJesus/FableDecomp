#include <stdio.h>

struct Event { unsigned int typeId; unsigned int payload; };

// 64-byte-stride handler table entry.
struct Handler {
    unsigned char pad0[0x10];
    void (__fastcall *fn)(void* self, void* edx, Event* ev); // +0x10
    unsigned int thisAdjust;                                 // +0x14
    unsigned char pad2[0x40 - 0x18];
};

// The global table the function-under-test dispatches through.
extern "C" Handler g_eventHandlerTable[8];
Handler g_eventHandlerTable[8];

// Function under test (naked transcription of retail 0x0049e1d0).
void __declspec(naked) __fastcall ProcessEvent(Event* ev)
{
    (void)ev;
    __asm {
        push esi
        mov  esi, ecx
        mov  ecx, dword ptr [esp+8]
        mov  eax, dword ptr [ecx]
        shl  eax, 6
        add  eax, offset g_eventHandlerTable
        mov  edx, dword ptr [eax+0x10]
        test edx, edx
        je   done
        push ecx
        mov  ecx, dword ptr [eax+0x14]
        add  ecx, esi
        call edx
    done:
        pop  esi
        ret  4
    }
}

// Observable state written by the handler.
static void*        g_seenSelf   = 0;
static Event*       g_seenEvent  = 0;
static unsigned int g_seenType   = 0;
static int          g_callCount  = 0;

static void __fastcall Handler3(void* self, void* /*edx*/, Event* ev)
{
    g_seenSelf  = self;
    g_seenEvent = ev;
    g_seenType  = ev->typeId;
    ++g_callCount;
}

int main()
{
    // Populate the table: only slot 3 has a live handler, with thisAdjust 0x20.
    for (int i = 0; i < 8; ++i) { g_eventHandlerTable[i].fn = 0; g_eventHandlerTable[i].thisAdjust = 0; }
    g_eventHandlerTable[3].fn = Handler3;
    g_eventHandlerTable[3].thisAdjust = 0x20;

    // Fake object; ProcessEvent adjusts this by +0x20 before calling.
    unsigned char obj[0x40];
    void* self = (void*)obj;

    // Case A: typeId 3 -> handler fires with adjusted self.
    Event evA; evA.typeId = 3; evA.payload = 0xABCD;
    __asm {
        mov  ecx, self
        lea  eax, evA
        push eax
        call ProcessEvent
    }
    if (g_callCount != 1)                         { printf("FAIL callCount=%d\n", g_callCount); return 1; }
    if (g_seenType != 3)                          { printf("FAIL type=%u\n", g_seenType); return 1; }
    if (g_seenEvent != &evA)                      { printf("FAIL event ptr\n"); return 1; }
    if (g_seenSelf != (void*)(obj + 0x20))        { printf("FAIL self adjust\n"); return 1; }

    // Case B: typeId 5 -> null handler -> no-op, no crash, count unchanged.
    Event evB; evB.typeId = 5; evB.payload = 0;
    __asm {
        mov  ecx, self
        lea  eax, evB
        push eax
        call ProcessEvent
    }
    if (g_callCount != 1)                         { printf("FAIL null-handler fired count=%d\n", g_callCount); return 1; }

    printf("OK_0x0049e1d0\n");
    return 0;
}
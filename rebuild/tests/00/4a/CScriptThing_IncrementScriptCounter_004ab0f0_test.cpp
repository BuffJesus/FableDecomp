#include <stdio.h>

// ---- Function under test (naked-asm transcription of 0x004ab0f0) ----
__declspec(naked) void IncrementScriptCounter()
{
    __asm {
        mov ecx, dword ptr [ecx+4]
        test ecx, ecx
        je   done
        mov  eax, dword ptr [ecx]
        jmp  dword ptr [eax+0x120]
    done:
        ret
    }
}

// ---- Test scaffolding: a fake object whose vtable slot 72 (offset 0x120)
//      points at a marker function that records it was called. ----
static int g_called = 0;

// __fastcall so 'this' arrives in ecx (matches jmp [eax+0x120] dispatch).
static void __fastcall Marker(void* self, void* /*edx*/)
{
    (void)self;
    g_called = 1;
}

int main()
{
    // Build a vtable with 128 slots; slot 72 (byte offset 0x120) = Marker.
    static void* vtbl[128] = {0};
    vtbl[72] = (void*)&Marker;   // 72*4 = 0x120

    // The counter object: first field is its vtable pointer.
    struct CounterObj { void* vptr; } counter;
    counter.vptr = vtbl;

    // The 'this' (CScriptThing): field at +0x4 is the counter object pointer.
    struct ScriptThing { void* pad0; void* field_4; } st;
    st.pad0 = 0;
    st.field_4 = &counter;

    // Case 1: field_4 non-null -> virtual dispatch fires.
    g_called = 0;
    {
        void* pThis = &st;
        __asm {
            mov ecx, pThis
            call IncrementScriptCounter
        }
    }
    if (g_called != 1) { printf("FAIL: virtual not dispatched\n"); return 1; }

    // Case 2: field_4 null -> no dispatch, must return cleanly.
    st.field_4 = 0;
    g_called = 0;
    {
        void* pThis = &st;
        __asm {
            mov ecx, pThis
            call IncrementScriptCounter
        }
    }
    if (g_called != 0) { printf("FAIL: dispatched on null field_4\n"); return 1; }

    printf("OK_0x004ab0f0\n");
    return 0;
}
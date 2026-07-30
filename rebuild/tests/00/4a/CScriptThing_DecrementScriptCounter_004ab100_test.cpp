#include <stdio.h>
#include <string.h>

__declspec(naked) void DecrementScriptCounter()
{
    __asm
    {
        mov ecx, dword ptr [ecx+0x4]
        test ecx, ecx
        je  done
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax+0x124]
    done:
        ret
    }
}

static int g_calls = 0;
static void* g_seen_this = 0;

static void __stdcall RecordCall(void* self)
{
    g_calls++;
    g_seen_this = self;
}

// virtual method at vtable byte offset 0x124 (__fastcall: this in ecx)
__declspec(naked) void VirtualSlot()
{
    __asm
    {
        push ecx
        call RecordCall
        ret
    }
}

static void* g_vtable[128];

struct Counter { void** vptr; };
struct ScriptThing { void* pad0; Counter* counter; };

static void call_dsc(void* self)
{
    __asm
    {
        mov ecx, self
        call DecrementScriptCounter
    }
}

int main()
{
    memset(g_vtable, 0, sizeof(g_vtable));
    g_vtable[0x124/4] = (void*)&VirtualSlot;

    Counter c;
    c.vptr = g_vtable;

    ScriptThing st;
    st.pad0 = 0;
    st.counter = &c;
    call_dsc(&st);
    if (g_calls != 1)      { printf("FAIL calls=%d\n", g_calls); return 1; }
    if (g_seen_this != (void*)&c) { printf("FAIL this=%p exp=%p\n", g_seen_this, (void*)&c); return 1; }

    ScriptThing st2;
    st2.pad0 = 0;
    st2.counter = 0;
    call_dsc(&st2);
    if (g_calls != 1)      { printf("FAIL null-branch calls=%d\n", g_calls); return 1; }

    printf("OK_0x004ab100 calls=%d\n", g_calls);
    return 0;
}
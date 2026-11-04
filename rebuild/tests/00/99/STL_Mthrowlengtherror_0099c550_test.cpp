#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include "fable_stlport_string.h"

// puts/abort replaced by recording versions; abort unwinds back via longjmp.
static jmp_buf g_abortJump;
static int g_putsCalls;
static int g_abortCalls;
static char g_message[32];

extern "C" int __cdecl puts(const char* text)
{
    ++g_putsCalls;
    strncpy(g_message, text, sizeof(g_message) - 1);
    return 0;
}

extern "C" void __cdecl abort(void)
{
    ++g_abortCalls;
    longjmp(g_abortJump, 1);
}

typedef _STL::_String_base<unsigned short, _STL::allocator<unsigned short> > WideBase;

int main()
{
    unsigned char storage[sizeof(WideBase)];
    memset(storage, 0x3C, sizeof(storage));
    if (setjmp(g_abortJump) == 0)
    {
        reinterpret_cast<const WideBase*>(storage)->_M_throw_length_error();
        return 1;  // must not return normally
    }
    if (g_putsCalls != 1 || g_abortCalls != 1) return 2;
    if (strcmp(g_message, "basic_string") != 0) return 3;
    if (storage[0] != 0x3C) return 4;
    printf("FABLETLC_STL_THROW_LENGTH_ERROR PASS\n");
    return 0;
}
// Fixture: CInputEvent::GetKey (0x00A03B70)
#include <stdio.h>
#include <string.h>
#include "fable_input_event.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

// Raw 0x34-byte event image; fields poked at their retail offsets.
static unsigned char g_ev[sizeof(CInputEvent) + 8];
static const CInputEvent* Event(unsigned char fill)
{
    memset(g_ev, fill, sizeof(g_ev));
    return reinterpret_cast<const CInputEvent*>(g_ev);
}
template <class T> static void Poke(unsigned off, T v) { memcpy(g_ev + off, &v, sizeof(v)); }

int main()
{
    const CInputEvent* e = Event(0xEE);
    Poke(0x00, (long)KB_Y); Poke(0x04, 0x12345678L); Poke(0x28, 1L);
    EXPECT(e->GetKey() == KB_Y);
    Poke(0x00, 0x42L);
    EXPECT((long)e->GetKey() == 0x42);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("PASS_00a03b70\n");
    return 0;
}

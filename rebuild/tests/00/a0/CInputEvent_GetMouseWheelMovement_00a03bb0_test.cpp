// Fixture: CInputEvent::GetMouseWheelMovement (0x00A03BB0)
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
    Poke(0x0C, 1.0f); Poke(0x10, 2.0f); Poke(0x14, -120.0f); Poke(0x18, 3.0f);
    EXPECT(e->GetMouseWheelMovement() == -120.0f);
    Poke(0x14, 0.5f);
    EXPECT(e->GetMouseWheelMovement() == 0.5f);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("OK_00a03bb0\n");
    return 0;
}

// Fixture: CInputEvent::GetChar (0x00A03B80)
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
    Poke(0x00, (unsigned short)0x00E9); Poke(0x02, (unsigned short)0xBEEF);
    EXPECT(e->GetChar() == 0x00E9);                  // 16-bit read: upper half ignored
    Poke(0x00, (unsigned short)0x4E2D);
    EXPECT(e->GetChar() == 0x4E2D);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("FABLETLC_INPUT_EVENT_GET_CHAR PASS\n");
    return 0;
}
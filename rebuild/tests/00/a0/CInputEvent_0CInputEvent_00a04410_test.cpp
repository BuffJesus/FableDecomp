// Fixture: CInputEvent::CInputEvent (0x00A04410): Type/DeviceNumber and the
// 0x20-byte data union are zeroed; DeviceType and the two times are left alone.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_input_event.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

int main()
{
    static unsigned char buf[sizeof(CInputEvent) + 8];
    memset(buf, 0xEE, sizeof(buf));
    new (buf) CInputEvent;
    for (unsigned i = 0; i < 0x20; ++i) EXPECT(buf[i] == 0);
    for (unsigned i = 0x20; i < 0x24; ++i) EXPECT(buf[i] == 0xEE);   // DeviceType untouched
    for (unsigned i = 0x24; i < 0x2C; ++i) EXPECT(buf[i] == 0);      // DeviceNumber, Type
    for (unsigned i = 0x2C; i < sizeof(buf); ++i) EXPECT(buf[i] == 0xEE);
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("FABLETLC_INPUT_EVENT_CTOR PASS\n");
    return 0;
}
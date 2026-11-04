// Fixture: CInputEvent::~CInputEvent (0x00A03B30): empty, writes nothing.
#include <stdio.h>
#include <string.h>
#include "fable_input_event.h"

int main()
{
    static unsigned char buf[sizeof(CInputEvent)];
    memset(buf, 0xEE, sizeof(buf));
    reinterpret_cast<CInputEvent*>(buf)->~CInputEvent();
    for (unsigned i = 0; i < sizeof(buf); ++i) if (buf[i] != 0xEE) { printf("FAIL byte %u\n", i); return 1; }
    printf("OK_00a03b30\n");
    return 0;
}

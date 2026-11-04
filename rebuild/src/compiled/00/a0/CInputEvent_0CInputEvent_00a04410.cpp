// CInputEvent::CInputEvent (retail 0x00A04410).
// FableWin.pdb ??0CInputEvent@@QAE@XZ; its debug body: Type = IE_NULL,
// DeviceNumber = 0, then memset of the 0x20-byte UInputEventData at +0x00.
#include <string.h>
#include "fable_input_event.h"

CInputEvent::CInputEvent()
{
    Type = IE_NULL;
    DeviceNumber = 0;
    memset(this, 0, 0x20);
}
// CGameEvent::AddData @ 0x009F1760 (FableWin ?AddData@CGameEvent@@QAEXJPBX@Z).
//
// Appends raw bytes at the event's write cursor and advances it (the cursor
// is a byte; callers stay within the 32-byte payload).

#include "fable_game_event.h"

#include <string.h>

void CGameEvent::AddData(long size, const void* data)
{
    memcpy(&Data[EndPos], data, size);
    EndPos += static_cast<unsigned char>(size);
}
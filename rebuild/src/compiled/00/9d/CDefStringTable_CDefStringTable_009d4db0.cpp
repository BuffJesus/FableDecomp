// CDefStringTable::CDefStringTable() @ 0x009D4DB0 (FableWin
// ??0CDefStringTable@@QAE@XZ). The unit also defines GDefStringTable (retail
// storage 0x013CA828; dynamic initializer 0x012240A0, atexit thunk 0x0122A780).
#include "fable_def_string.h"
#include "fable_crc.h"
#include "fable_win32_time.h"

CDefStringTable GDefStringTable;

// An empty, unlocked, writable table; its random id is the CRC of the
// current system time.
CDefStringTable::CDefStringTable()
    : StringCount(0), SharedCount(0), Locked(false), ReadOnly(false)
{
    SYSTEMTIME time;
    GetSystemTime(&time);
    RandomID = CCRC::Calc(0, &time, sizeof(time));
}
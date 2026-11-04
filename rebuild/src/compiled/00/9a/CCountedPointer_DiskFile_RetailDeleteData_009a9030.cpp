// Same 009A9030 callback as the generic counted disk owner, with the public
// declaration used by fable_retail_banks.h (SIX rather than private CIX).
#include "fable_retail_banks.h"
#include "fable_disk_file.h"
void FABLE_FASTCALL CCountedPointer<CDiskFileWin32>::DeleteData(void* data)
{
    delete static_cast<CDiskFileWin32*>(data);
}

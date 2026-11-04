// CFileDataInputStream::~CFileDataInputStream() @ 0x00994780 (FableWin
// ??1CFileDataInputStream@@UAE@XZ): closes the stream (freeing its buffer and
// leaving the file at the stream's position); the base destructors are inline.
#include "fable_data_input_stream.h"

CFileDataInputStream::~CFileDataInputStream()
{
    Close();
}

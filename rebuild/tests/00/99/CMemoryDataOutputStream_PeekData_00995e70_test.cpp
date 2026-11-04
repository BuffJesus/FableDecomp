// Behaviour of CMemoryDataOutputStream::PeekData() const 0x00995E70 (emitted
// out of line by the Write unit 0x00996CC0, which also expands it): the Data
// array object itself while nothing is written (CArray<char>::GetAsCArray's
// zero-size answer), the first written byte otherwise.
#include "fable_data_output_stream.h"
#include <stdio.h>
#include <string.h>

// Link scaffolding: the unit's Write grows Data through resize (never called here).
namespace _STL
{
    template <>
    void vector<char>::resize(size_t, const char&) {}
}

int main()
{
    static unsigned char store[sizeof(CMemoryDataOutputStream)];
    memset(store, 0, sizeof(store));
    CMemoryDataOutputStream* s = reinterpret_cast<CMemoryDataOutputStream*>(store);
    static char bytes[8] = "abcdefg";

    if (s->PeekData() != (const void*)&s->Data) { puts("never written"); return 1; }

    // Storage reserved but empty: still the array object.
    s->Data._M_start = s->Data._M_finish = bytes;
    s->Data._M_end_of_storage._M_data = bytes + sizeof(bytes);
    if (s->PeekData() != (const void*)&s->Data) { puts("reserved, empty"); return 1; }

    s->Data._M_finish = bytes + 3;
    if (s->PeekData() != (const void*)bytes) { puts("written"); return 1; }

    puts("FABLETLC_CMEMORYDATAOUTPUTSTREAM_PEEKDATA_00995E70_PASS");
    return 0;
}

#include <stdio.h>

typedef int D3DFORMAT;

// Behaviour model mirroring the retail walk: cursor starts at base; the format
// dword is at [cursor-8], the record marker at [cursor+0x24]; stride 0x24.
// Returns the 0-based index of the matching format, or -1 if the walk hits a
// marker == -1 before a match.

static int find(D3DFORMAT fmt, int* base)
{
    long index = 0;
    int* edx = base;
    for (;;)
    {
        if (edx[-2] == (int)fmt)
            return (int)index;
        edx += 9;
        ++index;
        if (edx[0] == -1)
            return -1;
    }
}

int main()
{
    // Build a table of records (9 ints each). Cursor points at record[i]+2 ints
    // (i.e. base = &buf[2]); format at [cursor-2], marker at [cursor+9].
    static int buf[9 * 8];
    for (int i = 0; i < 8 * 9; ++i) buf[i] = 0;

    // Lay out formats so that at cursor = &buf[2 + i*9], [cursor-2] = buf[i*9].
    // record i format lives at buf[i*9].
    for (int i = 0; i < 8; ++i)
        buf[i * 9] = 100 + i;

    // Marker at [cursor + 9] for cursor at record i = buf[2 + i*9 + 9] = buf[11 + i*9]
    // which is record (i+1)'s buf[(i+1)*9 + 2]. Set record 6's that slot to -1 to
    // terminate the walk after record 5 on a miss.
    buf[2 + 6 * 9] = -1; // = buf[56]

    int* base = &buf[2]; // cursor for record 0

    int r0 = find(100, base);   // match at index 0
    int r3 = find(103, base);   // match at index 3
    int rmiss = find(9999, base); // no match -> hits marker -1 -> -1

    if (r0 == 0 && r3 == 3 && rmiss == -1)
        printf("BEHAVIOR_PASS\n");
    else
        printf("BEHAVIOR_FAIL r0=%d r3=%d rmiss=%d\n", r0, r3, rmiss);
    return 0;
}
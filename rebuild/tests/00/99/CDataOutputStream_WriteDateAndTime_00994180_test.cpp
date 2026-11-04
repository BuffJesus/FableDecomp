#include <stdio.h>
#include <string.h>
#include "fable_data_output_stream.h"
#include "fable_date_and_time.h"
static CDataOutputStream* expected;
static unsigned calls, count;
static unsigned char bytes[12];
static long sizes[7];
static bool valid = true;
static void __fastcall Write(CDataOutputStream* self, void*, const void* data, long size) {
    if (self != expected || calls >= 7 || size < 0 || count + size > 12) { valid = false; return; }
    sizes[calls++] = size; memcpy(bytes + count, data, size); count += size;
}
CDateAndTime::CDateAndTime() {}
int main() {
    void* vtable[5] = {0}; vtable[4] = reinterpret_cast<void*>(&Write);
    unsigned long storage[2]; storage[0] = reinterpret_cast<unsigned long>(vtable); storage[1] = 0xA5A5A5A5;
    expected = reinterpret_cast<CDataOutputStream*>(storage);
    CDateAndTime date;
    date.Year = -7; date.Month = 0x1AB; date.Day = 0x234; date.Hour = 0x3CD; date.Minute = 0x456;
    date.Second = 0x11223344; date.Millisecond = 0x55667788;
    unsigned char original[sizeof(date)]; memcpy(original, &date, sizeof(date));
    expected->WriteDateAndTime(date);
    const unsigned char wanted[12] = {0xF9,0xFF,0xFF,0xFF,0xAB,0x34,0xCD,0x56,0x44,0x33,0x88,0x77};
    const long wantedSizes[7] = {4,1,1,1,1,2,2};
    if (!valid || calls != 7 || count != 12 || memcmp(bytes, wanted, 12) || memcmp(sizes, wantedSizes, sizeof(sizes))) return 1;
    if (memcmp(original, &date, sizeof(date)) || storage[1] != 0xA5A5A5A5) return 2;
    puts("WRITE_DATE_00994180_PASS"); return 0;
}

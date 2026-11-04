#include <stdio.h>
#include <string.h>
#include "fable_data_output_stream.h"
char g_FableEmptyNarrowString_0129AAF4[1] = {0};
static CDataOutputStream* expectedStream;
static const void* lastData;
static long lastSize;
static unsigned calls;
static bool valid = true;
static void __fastcall Capture(CDataOutputStream* self, void*, const void* data, long size) {
    valid = valid && self == expectedStream; lastData = data; lastSize = size; ++calls;
}
int main() {
    void* vtable[5] = {0}; vtable[4] = reinterpret_cast<void*>(&Capture);
    unsigned long stream[2] = {reinterpret_cast<unsigned long>(vtable), 0xA5A5A5A5};
    expectedStream = reinterpret_cast<CDataOutputStream*>(stream);
    CCharStringData* storage = 0;
    CCharString& text = *reinterpret_cast<CCharString*>(&storage);
    expectedStream->WriteNullTerminatedString(text);
    if (!valid || calls != 1 || lastData != g_FableEmptyNarrowString_0129AAF4 || lastSize != 1) return 1;
    CCharStringData data;
    char payload[] = {'A',0,'B',0};
    data.Data.PData = payload; data.Data.StringLength = 3; storage = &data;
    expectedStream->WriteNullTerminatedString(text);
    if (!valid || calls != 2 || lastData != payload || lastSize != 4) return 2;
    data.Data.StringLength = 0;
    expectedStream->WriteNullTerminatedString(text);
    if (!valid || calls != 3 || lastData != payload || lastSize != 1 || storage != &data || stream[1] != 0xA5A5A5A5) return 3;
    puts("WRITE_STRING_00995ae0_PASS"); return 0;
}

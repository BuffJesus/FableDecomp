#include <stdio.h>
#include <string.h>
#include "fable_file_data_output_stream.h"
static CAFile* expectedFile;
static unsigned calls;
static long position;
static const void* data;
static long size;
static bool valid = true;
static void __fastcall Set(CAFile* self, void*, long value) { valid = valid && self == expectedFile; position = value; ++calls; }
static long __fastcall Get(const CAFile* self, void*) { valid = valid && self == expectedFile; ++calls; return position; }
static void __fastcall Write(CAFile* self, void*, const void* value, long bytes, CASuspendableProcess* process) {
    valid = valid && self == expectedFile && process == 0; data = value; size = bytes; ++calls;
}
static void __fastcall Close(CAFile*, void*) { ++calls; }
int main() {
    void* vtable[8] = {0};
    vtable[2] = reinterpret_cast<void*>(&Close);
    vtable[4] = reinterpret_cast<void*>(&Write);
    vtable[5] = reinterpret_cast<void*>(&Set);
    vtable[7] = reinterpret_cast<void*>(&Get);
    void** fileVtable = vtable;
    expectedFile = reinterpret_cast<CAFile*>(&fileVtable);
    unsigned char raw[sizeof(CFileDataOutputStream)]; memset(raw, 0xA5, sizeof(raw));
    CFileDataOutputStream* stream = reinterpret_cast<CFileDataOutputStream*>(raw);
    stream->File = expectedFile;
    position = -1;
    if (stream->CFileDataOutputStream::GetPosition() != 0xFFFFFFFF || calls != 1 || !valid) return 1;
    position = 42;
    if (stream->CFileDataOutputStream::GetPosition() != 42 || calls != 2 || !valid) return 2;
    puts("FILE_OUTPUT_00994460_PASS"); return 0;
}

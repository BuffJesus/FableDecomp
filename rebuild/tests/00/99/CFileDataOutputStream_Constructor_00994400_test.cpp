#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_file_data_output_stream.h"
static unsigned baseCalls, stringCalls;
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++baseCalls; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::CCharString(const char* text, long length) : storage_(0) {
    if (text && strcmp(text, "\r\n") == 0 && length == -1) ++stringCalls;
}
CCharString::~CCharString() {}
CFileDataOutputStream::~CFileDataOutputStream() {}
void CFileDataOutputStream::SetPosition(unsigned long) {}
unsigned long CFileDataOutputStream::GetPosition() const { return 0; }
void CFileDataOutputStream::Close() {}
void CFileDataOutputStream::Write(const void*, long) {}
int main() {
    unsigned char storage[sizeof(CFileDataOutputStream) + 4];
    memset(storage, 0xA5, sizeof(storage));
    CAFile* file = reinterpret_cast<CAFile*>(0x12345678);
    CFileDataOutputStream* stream = new (storage) CFileDataOutputStream(file);
    if (stream != reinterpret_cast<CFileDataOutputStream*>(storage) || stream->File != file || baseCalls != 1 || stringCalls != 1) return 1;
    if (*reinterpret_cast<unsigned long*>(storage) == 0xA5A5A5A5 || *reinterpret_cast<unsigned long*>(storage + 4)) return 2;
    for (unsigned i = sizeof(CFileDataOutputStream); i < sizeof(storage); ++i) if (storage[i] != 0xA5) return 3;
    puts("FILE_OUTPUT_CTOR_00994400_PASS"); return 0;
}

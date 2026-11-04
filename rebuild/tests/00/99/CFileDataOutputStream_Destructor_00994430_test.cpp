#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include "fable_file_data_output_stream.h"
static char events[8]; static unsigned count;
static void* heapObject;
static void** savedVtable;
void operator delete(void* p) throw() {
    if (p == heapObject) events[count++] = 'D';
    free(p);
}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { events[count++] = 'B'; }
CCharString::~CCharString() { savedVtable = *reinterpret_cast<void***>(reinterpret_cast<char*>(this) - 4); events[count++] = 'N'; }
void CFileDataOutputStream::SetPosition(unsigned long) {}
unsigned long CFileDataOutputStream::GetPosition() const { return 0; }
void CFileDataOutputStream::Close() {}
void CFileDataOutputStream::Write(const void*, long) {}
int main() {
    unsigned char storage[sizeof(CFileDataOutputStream)]; memset(storage, 0xA5, sizeof(storage));
    CFileDataOutputStream* stream = reinterpret_cast<CFileDataOutputStream*>(storage);
    stream->CFileDataOutputStream::~CFileDataOutputStream();
    if (count != 2 || events[0] != 'N' || events[1] != 'B') return 1;
    if (reinterpret_cast<unsigned long>(stream->File) != 0xA5A5A5A5) return 2;
    count = 0;
    heapObject = malloc(sizeof(CFileDataOutputStream));
    memset(heapObject, 0xA5, sizeof(CFileDataOutputStream));
    *reinterpret_cast<void***>(heapObject) = savedVtable;
    delete reinterpret_cast<CFileDataOutputStream*>(heapObject);
    if (count != 3 || memcmp(events, "NBD", 3)) return 3;
    puts("FILE_OUTPUT_DTOR_00994430_PASS"); return 0;
}

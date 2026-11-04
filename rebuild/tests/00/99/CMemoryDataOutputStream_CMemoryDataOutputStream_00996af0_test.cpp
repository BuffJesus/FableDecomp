// Behaviour of CMemoryDataOutputStream::CMemoryDataOutputStream() 0x00996AF0:
// runs CBaseClassNonCopyable's ctor on the object, builds NewLine from
// "\r\n" (length -1, i.e. measured), empties Data, sets the position to 0 and
// leaves the object with the memory stream's vtable (its SetPosition,
// GetPosition, Close and Write in slots 1-4).
#include "fable_data_output_stream.h"
#include "fable_data_output_stream_inline.h"
#include <new>
#include <stdio.h>
#include <string.h>

static char g_log[256];
static void Log(const char* t) { strcat(g_log, t); }
static const void* g_object;

CBaseClassNonCopyable::CBaseClassNonCopyable() { if (this == g_object) Log("base;"); }
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
static const char* g_newLineText;
static long g_newLineLength;
CCharString::CCharString(const char* text, long length)
{
    if ((const char*)this == (const char*)g_object + 4) Log("newline;");
    g_newLineText = text; g_newLineLength = length;
}
CCharString::~CCharString() {}

CMemoryDataOutputStream::~CMemoryDataOutputStream() {}
void CMemoryDataOutputStream::SetPosition(unsigned long) { Log("setpos;"); }
unsigned long CMemoryDataOutputStream::GetPosition() const { Log("getpos;"); return CurrentPosition; }
void CMemoryDataOutputStream::Close() { Log("close;"); }
void CMemoryDataOutputStream::Write(const void*, long size) { char t[16]; sprintf(t, "write%ld;", size); Log(t); }

int main()
{
    static unsigned char store[sizeof(CMemoryDataOutputStream)];
    memset(store, 0xCC, sizeof(store));
    g_object = store;
    CMemoryDataOutputStream* s = new (store) CMemoryDataOutputStream;
    if (strcmp(g_log, "base;newline;") != 0) { printf("order: %s\n", g_log); return 1; }
    if (strcmp(g_newLineText, "\r\n") != 0 || g_newLineLength != -1) { puts("newline"); return 1; }
    if (s->Data._M_start != 0 || s->Data._M_finish != 0 || s->Data._M_end_of_storage._M_data != 0 ||
        s->CurrentPosition != 0) { puts("fields"); return 1; }
    g_log[0] = 0;
    CDataOutputStream* d = s;
    d->SetPosition(3);
    s->CurrentPosition = 9;
    if (d->GetPosition() != 9) { puts("getpos"); return 1; }
    d->Close();
    d->WriteULONG(5);
    if (strcmp(g_log, "setpos;getpos;close;write4;") != 0) { printf("slots: %s\n", g_log); return 1; }
    puts("FABLETLC_CMEMORYDATAOUTPUTSTREAM_CTOR_00996AF0_PASS");
    return 0;
}
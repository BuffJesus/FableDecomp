// Behaviour fixture for CMemoryDataOutputStream's SetPosition, GetPosition,
// Close and destructor (plus the scalar deleting destructor its unit emits).
// The members are the real compiled providers; the constructor is the landed
// one. CCharString's constructor/destructor and the CBaseClass /
// CBaseClassNonCopyable constructors/destructors are fixture-side recorders.
#include <new>
#define private public
#include "fable_string.h"
#undef private
#include "fable_data_output_stream.h"
#include "fable_file_exception.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_stringCtors, g_stringDtors, g_baseCtors, g_baseDtors;
static const void* g_lastStringDtor;
static const void* g_lastBaseDtor;
static void* g_vptrAtBaseDtor;
CCharString::CCharString(const char*, long) : storage_(0) { ++g_stringCtors; }
CCharString::~CCharString() { ++g_stringDtors; g_lastStringDtor = this; }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
// The vtable's Write slot (0x00996CC0, landed separately); not reached here.
void CMemoryDataOutputStream::Write(const void*, long) {}
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_baseCtors; }
CBaseClassNonCopyable::~CBaseClassNonCopyable()
{
    ++g_baseDtors;
    g_lastBaseDtor = this;
    g_vptrAtBaseDtor = *reinterpret_cast<void**>(this);
}

static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("MEMORY_OUTPUT_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)

int main()
{
    int cases = 0;
    static union { double align; unsigned char bytes[sizeof(CMemoryDataOutputStream)]; } storage;
    CMemoryDataOutputStream* s = reinterpret_cast<CMemoryDataOutputStream*>(&storage);
    static char buffer[64];
    static const unsigned long sizes[] = { 0, 1, 7, 64 };
    for (int k = 0; k < 4; ++k)
    {
        const unsigned long size = sizes[k];
        memset(&storage, 0x3c, sizeof(storage));
        s->Data._M_start = buffer;
        s->Data._M_finish = buffer + size;
        s->Data._M_end_of_storage._M_data = buffer + 64;
        static const unsigned long probes[] = { 0, 1, 6, 7, 8, 63, 64, 65, 0x7fffffff, 0x80000000, 0xffffffff };
        for (int p = 0; p < 11; ++p)
        {
            const unsigned long position = probes[p];
            s->CurrentPosition = 0x1234;
            bool threw = false;
            try
            {
                s->CMemoryDataOutputStream::SetPosition(position);
            }
            catch (CBBBFileException& e)
            {
                threw = true;
                CHECK(e.Type == CBBBFileException::BAD_SEEK, 1);
            }
            CHECK(threw == (position > size), 2);
            CHECK(s->CurrentPosition == (threw ? 0x1234 : position), 3);
            CHECK(s->CMemoryDataOutputStream::GetPosition() == s->CurrentPosition, 4);
            ++cases;
        }
        unsigned char before[sizeof(storage)];
        memcpy(before, &storage, sizeof(storage));
        s->CMemoryDataOutputStream::Close();
        CHECK(memcmp(before, &storage, sizeof(storage)) == 0, 5);
    }

    // Destructor on a stream that owns a block: frees it, destroys NewLine,
    // then runs the base destructor with the CDataStream vtable in place.
    for (int owned = 0; owned < 2; ++owned)
    {
        memset(&storage, 0, sizeof(storage));
        void* vptr = 0;
        {
            CMemoryDataOutputStream* made = new (static_cast<void*>(&storage)) CMemoryDataOutputStream;
            vptr = *reinterpret_cast<void**>(made);
            CHECK(made->CMemoryDataOutputStream::GetPosition() == 0 && made->Data._M_start == 0, 6);
            if (owned)
            {
                made->Data._M_start = static_cast<char*>(malloc(16));
                made->Data._M_finish = made->Data._M_start + 3;
                made->Data._M_end_of_storage._M_data = made->Data._M_start + 16;
            }
            g_stringDtors = g_baseDtors = 0;
            made->CMemoryDataOutputStream::~CMemoryDataOutputStream();
        }
        CHECK(g_stringDtors == 1 && g_lastStringDtor == &s->NewLine, 7);
        CHECK(g_baseDtors == 1 && g_lastBaseDtor == s && g_vptrAtBaseDtor != vptr && g_vptrAtBaseDtor != 0, 8);
        ++cases;
    }
    // Deleting through the base: the scalar deleting destructor.
    for (int owned = 0; owned < 2; ++owned)
    {
        g_stringCtors = g_stringDtors = g_baseCtors = g_baseDtors = 0;
        CDataOutputStream* heap = new CMemoryDataOutputStream;
        CHECK(g_stringCtors == 1 && g_baseCtors == 1, 9);
        if (owned)
        {
            CMemoryDataOutputStream* m = static_cast<CMemoryDataOutputStream*>(heap);
            m->Data._M_start = static_cast<char*>(malloc(8));
            m->Data._M_finish = m->Data._M_start;
            m->Data._M_end_of_storage._M_data = m->Data._M_start + 8;
        }
        delete heap;
        CHECK(g_stringDtors == 1 && g_baseDtors == 1 && g_lastBaseDtor == heap, 10);
        ++cases;
    }
    if (g_fail) return 1;
    printf("MEMORY_OUTPUT_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}

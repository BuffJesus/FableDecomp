#include <stdio.h>
#include <string.h>

#include "fable_data_output_stream.h"

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::CCharString(const char*, long) : storage_(0) {}
CCharString::~CCharString() {}

struct TestOutput : CDataOutputStream
{
    unsigned char captured[4];
    long sizes[4];
    int calls;

    TestOutput() : calls(0) { memset(captured, 0xff, sizeof(captured)); }
    virtual void SetPosition(unsigned long) {}
    virtual unsigned long GetPosition() const { return 0; }
    virtual void Close() {}
    virtual void Write(const void* data, long size)
    {
        if (calls < 4)
        {
            captured[calls] = *(const unsigned char*)data;
            sizes[calls] = size;
        }
        ++calls;
    }
};

int main()
{
    TestOutput output;
    output.WriteEBOOL(true);
    output.WriteEBOOL(false);
    bool noncanonical;
    *(unsigned char*)&noncanonical = 0x7f;
    output.WriteEBOOL(noncanonical);
    if (output.calls != 3 || output.captured[0] != 1 ||
        output.captured[1] != 0 || output.captured[2] != 1 ||
        output.sizes[0] != 1 || output.sizes[1] != 1 || output.sizes[2] != 1)
        return 1;
    puts("WriteEBOOL_OK");
    return 0;
}

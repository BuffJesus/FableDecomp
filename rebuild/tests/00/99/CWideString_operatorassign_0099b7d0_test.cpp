#include <stdio.h>

// Self-contained behavioral harness (/Od) mirroring the retail refcounted
// copy-on-write operator=. Release() is modeled directly (it is masked in
// the parity check as a relocation-relative call in the real binary), but
// its OBSERVABLE effect -- decrementing/freeing the previously-held data
// when this->data was non-null -- is reproduced so the behavior test can
// verify refcount bookkeeping end to end.

namespace
{
    int g_releaseCalls = 0;
    int g_liveBlocks = 0;
}

class CWideStringData
{
public:
    unsigned short* Chars;
    unsigned long RefCount;
};

class CWideString
{
public:
    CWideString() : data(0) {}

    void Adopt(CWideStringData* d)
    {
        data = d;
        ++data->RefCount;
    }

    CWideStringData* Raw() const { return data; }

    CWideString& operator=(const CWideString& rhs);

    void Release()
    {
        ++g_releaseCalls;
        if (data != 0)
        {
            --data->RefCount;
        }
        data = 0;
    }

private:
    CWideStringData* data;
};

CWideString& CWideString::operator=(const CWideString& rhs)
{
    if (&rhs != this)
    {
        CWideStringData* oldData = data;
        if (rhs.data != oldData)
        {
            if (oldData != 0)
                Release();
            CWideStringData* newData = rhs.data;
            if (newData != 0)
            {
                data = newData;
                ++newData->RefCount;
            }
        }
    }
    return *this;
}

CWideStringData* MakeBlock()
{
    CWideStringData* d = new CWideStringData();
    d->Chars = 0;
    d->RefCount = 0;
    ++g_liveBlocks;
    return d;
}

int main()
{
    // Case 1: plain assign, refcount goes from 1 (a alone) to 2 (shared).
    CWideStringData* blockA = MakeBlock();
    CWideString a;
    a.Adopt(blockA); // refcount = 1

    CWideString b;
    b = a; // rhs.data (blockA) != this->data (0) -> adopt, refcount = 2

    if (b.Raw() != blockA) return 1;
    if (blockA->RefCount != 2) return 2;

    // Case 2: self-assign must be a no-op (guarded by &rhs == this).
    int releasesBefore = g_releaseCalls;
    b = b;
    if (b.Raw() != blockA) return 3;
    if (blockA->RefCount != 2) return 4;
    if (g_releaseCalls != releasesBefore) return 5;

    // Case 3: assigning identical data (same pointer, different objects)
    // must also be a no-op on the data-identity branch.
    CWideString c;
    c.Adopt(blockA); // refcount = 3
    c = a; // rhs.data == this->data (blockA) -> skip entirely
    if (blockA->RefCount != 3) return 6;

    // Case 4: assign a different, non-null block over an existing one ->
    // Release() must fire exactly once on the old data.
    CWideStringData* blockB = MakeBlock();
    CWideString d;
    d.Adopt(blockB); // blockB refcount = 1
    int releasesBefore2 = g_releaseCalls;
    d = a; // releases blockB, adopts blockA
    if (g_releaseCalls != releasesBefore2 + 1) return 7;
    if (d.Raw() != blockA) return 8;
    if (blockA->RefCount != 4) return 9;
    if (blockB->RefCount != 0) return 10;

    // Case 5: assigning a null-data rhs onto a non-null this releases the
    // old data and leaves this->data null (no adopt branch taken).
    CWideString e; // data = 0
    CWideString f;
    f.Adopt(blockA); // refcount = 5
    int releasesBefore3 = g_releaseCalls;
    f = e; // rhs.data(0) != this->data(blockA) -> Release old, newData==0 -> no adopt
    if (g_releaseCalls != releasesBefore3 + 1) return 11;
    if (f.Raw() != 0) return 12;
    if (blockA->RefCount != 4) return 13;

    // Clean up remaining references so we don't leak in the test itself.
    a.Release();
    b.Release();
    c.Release();
    d.Release();
    e.Release();
    f.Release();

    if (blockA->RefCount != 0) return 14;

    puts("FABLETLC_CWIDESTRING_OPASSIGN_BEHAVIOR PASS");
    return 0;
}
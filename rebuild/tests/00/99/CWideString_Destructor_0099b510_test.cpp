#include <stdio.h>
#include <stdlib.h>

typedef unsigned int fable_u32;
typedef signed int fable_i32;

struct CWideStringData
{
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;

class TestWideString
{
public:
    TestWideString() : storage_(0) { ++g_CWideStringInstanceCount_013BCA20; }
    ~TestWideString();

    CWideStringData* storage_;
};

TestWideString::~TestWideString()
{
    CWideStringData* storage = storage_;

    if (storage != 0)
    {
        --storage->owners;
        if (storage->owners <= 0)
        {
            storage = storage_;
            if (storage != 0)
            {
                if (storage->text != 0)
                    free(storage->text);
                delete storage;
            }
        }
        storage_ = 0;
    }

    --g_CWideStringInstanceCount_013BCA20;
}

static int VerifyNullStorageRelease()
{
    g_CWideStringInstanceCount_013BCA20 = 0;
    {
        TestWideString value;
        if (g_CWideStringInstanceCount_013BCA20 != 1)
            return 1;
    }

    if (g_CWideStringInstanceCount_013BCA20 != 0)
        return 2;
    return 0;
}

static int VerifySharedStorageRelease()
{
    CWideStringData* storage = new CWideStringData;
    storage->text = 0;
    storage->unknown04 = 0;
    storage->unknown08 = 0;
    storage->owners = 2;

    {
        TestWideString value;
        value.storage_ = storage;
    }

    if (storage->owners != 1)
    {
        delete storage;
        return 3;
    }
    delete storage;

    if (g_CWideStringInstanceCount_013BCA20 != 0)
        return 4;
    return 0;
}

static int VerifySoleOwnerRelease()
{
    CWideStringData* storage = new CWideStringData;
    storage->text = static_cast<wchar_t*>(malloc(16 * sizeof(wchar_t)));
    storage->unknown04 = 0;
    storage->unknown08 = 0;
    storage->owners = 1;

    {
        TestWideString value;
        value.storage_ = storage;
    }

    if (g_CWideStringInstanceCount_013BCA20 != 0)
        return 5;
    return 0;
}

int main()
{
    int result = VerifyNullStorageRelease();
    if (result == 0)
        result = VerifySharedStorageRelease();
    if (result == 0)
        result = VerifySoleOwnerRelease();

    if (result != 0)
    {
        printf("FABLETLC_WIDE_STRING_DESTRUCTOR_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_WIDE_STRING_DESTRUCTOR_BEHAVIOR PASS\n");
    return 0;
}
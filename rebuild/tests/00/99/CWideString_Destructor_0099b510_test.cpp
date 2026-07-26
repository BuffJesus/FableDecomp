#include <stdio.h>
#include <stdlib.h>

#include "fable_filesystem.h"

namespace
{
    struct TestStringStorage
    {
        wchar_t* text;
        fable_u32 unknown04;
        fable_u32 unknown08;
        fable_i32 owners;
    };
}

fable_i32 g_CStringInstanceCount_013BCA20 = 0;

static void InstallStorage(
    CWideString& value,
    TestStringStorage* storage)
{
    *reinterpret_cast<fable_u32*>(&value) =
        reinterpret_cast<fable_u32>(storage);
}

static int VerifyNullStorageRelease()
{
    g_CStringInstanceCount_013BCA20 = 0;
    {
        CWideString value;
        if (g_CStringInstanceCount_013BCA20 != 1)
            return 1;
    }

    if (g_CStringInstanceCount_013BCA20 != 0)
        return 2;
    return 0;
}

static int VerifySharedStorageRelease()
{
    TestStringStorage* storage = new TestStringStorage;
    storage->text = 0;
    storage->unknown04 = 0;
    storage->unknown08 = 0;
    storage->owners = 2;

    {
        CWideString value;
        InstallStorage(value, storage);
    }

    if (storage->owners != 1)
    {
        delete storage;
        return 3;
    }
    delete storage;

    if (g_CStringInstanceCount_013BCA20 != 0)
        return 4;
    return 0;
}

static int VerifySoleOwnerRelease()
{
    TestStringStorage* storage = new TestStringStorage;
    storage->text = static_cast<wchar_t*>(malloc(16 * sizeof(wchar_t)));
    storage->unknown04 = 0;
    storage->unknown08 = 0;
    storage->owners = 1;

    {
        CWideString value;
        InstallStorage(value, storage);
    }

    if (g_CStringInstanceCount_013BCA20 != 0)
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

#include <stdio.h>
#include <string.h>

#include "../../../src/compiled/00/9e/CQuickDrawTriInfoVector_Erase_009e15e0.cpp"

static fable_i32 g_TextureDestroyCount;
static fable_i32 g_StringDestroyCount;

CWideStringEraseView::~CWideStringEraseView()
{
    ++g_StringDestroyCount;
}

void CTextureEraseView::Uninitialise()
{
    ++g_TextureDestroyCount;
}

CQuickDrawTriInfoEraseView* __fastcall
FableCopyQuickDrawRange(
    CQuickDrawTriInfoEraseView* first,
    CQuickDrawTriInfoEraseView* last,
    CQuickDrawTriInfoEraseView* destination,
    CQuickDrawTriInfoEraseView**,
    fable_i32)
{
    while (first != last)
    {
        *destination++ = *first++;
    }
    return destination;
}

static void Fill(
    CQuickDrawTriInfoEraseView& value,
    fable_u8 marker)
{
    memset(&value, marker, sizeof(value));
}

int main()
{
    CQuickDrawTriInfoEraseView values[5];
    for (fable_i32 index = 0; index < 5; ++index)
    {
        Fill(values[index], static_cast<fable_u8>(0x20 + index));
    }

    CQuickDrawTriInfoVectorView vector;
    vector.begin00 = values;
    vector.end04 = values + 5;
    vector.capacity08 = values + 5;
    g_TextureDestroyCount = 0;
    g_StringDestroyCount = 0;

    CQuickDrawTriInfoEraseView* result =
        vector.Erase(values + 1, values + 3);
    if (result != values + 1 ||
        vector.end04 != values + 3 ||
        values[1].unknown00[0] != 0x23 ||
        values[2].unknown00[0] != 0x24 ||
        g_TextureDestroyCount != 2 ||
        g_StringDestroyCount != 2)
    {
        printf("FABLETLC_QUICKDRAW_ERASE_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    g_TextureDestroyCount = 0;
    g_StringDestroyCount = 0;
    result = vector.Erase(values + 1, values + 1);
    if (result != values + 1 ||
        vector.end04 != values + 3 ||
        g_TextureDestroyCount != 0 ||
        g_StringDestroyCount != 0)
    {
        printf("FABLETLC_QUICKDRAW_ERASE_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    g_TextureDestroyCount = 0;
    g_StringDestroyCount = 0;
    result = vector.Erase(values + 1, values + 3);
    if (result != values + 1 ||
        vector.end04 != values + 1 ||
        g_TextureDestroyCount != 2 ||
        g_StringDestroyCount != 2)
    {
        printf("FABLETLC_QUICKDRAW_ERASE_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_QUICKDRAW_ERASE_BEHAVIOR PASS\n");
    return 0;
}

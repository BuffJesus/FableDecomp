#include "rebuild_abi.h"

struct CWideStringEraseView
{
    void* data00;

    ~CWideStringEraseView();
};

struct CTextureEraseView
{
    fable_u8 unknown00[8];

    void Uninitialise();
};

struct CQuickDrawTriInfoEraseView
{
    fable_u8 unknown00[0x14];
    CWideStringEraseView name14;
    CTextureEraseView texture18;
    fable_u8 unknown20[0x1C];
};

CQuickDrawTriInfoEraseView* __fastcall
FableCopyQuickDrawRange(
    CQuickDrawTriInfoEraseView* first,
    CQuickDrawTriInfoEraseView* last,
    CQuickDrawTriInfoEraseView* destination,
    CQuickDrawTriInfoEraseView** checkedDestination,
    fable_i32 nonscalarTag);

struct CQuickDrawTriInfoVectorView
{
    CQuickDrawTriInfoEraseView* begin00;
    CQuickDrawTriInfoEraseView* end04;
    CQuickDrawTriInfoEraseView* capacity08;

    __forceinline void Destroy(
        CQuickDrawTriInfoEraseView* first,
        CQuickDrawTriInfoEraseView* last)
    {
        while (first != last)
        {
            first->texture18.Uninitialise();
            first->name14.~CWideStringEraseView();
            ++first;
        }
    }

    CQuickDrawTriInfoEraseView* Erase(
        CQuickDrawTriInfoEraseView* first,
        CQuickDrawTriInfoEraseView* last);
};

CQuickDrawTriInfoEraseView*
CQuickDrawTriInfoVectorView::Erase(
    CQuickDrawTriInfoEraseView* first,
    CQuickDrawTriInfoEraseView* last)
{
    CQuickDrawTriInfoVectorView* const activeVector = this;
    CQuickDrawTriInfoEraseView* const result = first;
    CQuickDrawTriInfoEraseView* const newEnd =
        FableCopyQuickDrawRange(
            last,
            activeVector->end04,
            first,
            &first,
            0);
    if (newEnd != activeVector->end04)
    {
        Destroy(newEnd, activeVector->end04);
        activeVector->end04 = newEnd;
        return first;
    }
    activeVector->end04 = newEnd;
    return result;
}

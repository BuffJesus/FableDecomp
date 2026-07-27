#include <stdio.h>

#if defined(_MSC_VER)
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif

struct CProgressDisplay
{
    int marker;
};

struct FableReferenceCount
{
    long owners;
    void* destroy;
    void* object;
};

struct CCountedProgressDisplay
{
    CProgressDisplay* object;
    FableReferenceCount* reference;
};

CProgressDisplay* g_FableProgressDisplayObject_013CAA38 = 0;
FableReferenceCount* g_FableProgressDisplayReference_013CAA3C = 0;

CCountedProgressDisplay* FABLE_FASTCALL GetProgressDisplay(
    CCountedProgressDisplay* result);

int main()
{
    CProgressDisplay display = {0x12345678};
    FableReferenceCount reference = {2, 0, &display};
    g_FableProgressDisplayObject_013CAA38 = &display;
    g_FableProgressDisplayReference_013CAA3C = &reference;

    CCountedProgressDisplay result = {};
    if (
        GetProgressDisplay(&result) != &result ||
        result.object != &display ||
        result.reference != &reference ||
        reference.owners != 3)
    {
        return 1;
    }

    g_FableProgressDisplayObject_013CAA38 = 0;
    g_FableProgressDisplayReference_013CAA3C = 0;
    CCountedProgressDisplay empty = {};
    if (
        GetProgressDisplay(&empty) != &empty ||
        empty.object != 0 ||
        empty.reference != 0 ||
        reference.owners != 3)
    {
        return 2;
    }

    printf("FABLETLC_GET_PROGRESS_DISPLAY_BEHAVIOR PASS\n");
    return 0;
}

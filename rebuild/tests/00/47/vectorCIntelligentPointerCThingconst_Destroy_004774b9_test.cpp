#include <stdio.h>
#include <string.h>

// Independent behaviour test for vecCIP_Destroy (vector::_Destroy @ 0x004774b9).

static int g_calls = 0;
static int g_lastFlag = -999;

struct CIntelligentPointer_CThing
{
    virtual void slot0(int flag);
    char pad[0x88 - 4];
};

void __fastcall vecCIP_Destroy(CIntelligentPointer_CThing* first,
                               CIntelligentPointer_CThing* last);

// Provide a concrete slot0 so linking works and we can observe calls.
void CIntelligentPointer_CThing::slot0(int flag)
{
    g_calls++;
    g_lastFlag = flag;
}

int main()
{
    // Build an array of 5 elements laid out contiguously with 0x88 stride.
    const int N = 5;
    CIntelligentPointer_CThing arr[N];

    // sanity: sizeof must be 0x88 for stride correctness
    if (sizeof(CIntelligentPointer_CThing) != 0x88) {
        printf("BAD_SIZE %u\n", (unsigned)sizeof(CIntelligentPointer_CThing));
        return 1;
    }

    // Destroy full range.
    vecCIP_Destroy(&arr[0], &arr[N]);
    if (g_calls != N || g_lastFlag != 0) {
        printf("FAIL calls=%d flag=%d\n", g_calls, g_lastFlag);
        return 1;
    }

    // Empty range: no calls.
    g_calls = 0; g_lastFlag = -999;
    vecCIP_Destroy(&arr[2], &arr[2]);
    if (g_calls != 0) {
        printf("FAIL empty calls=%d\n", g_calls);
        return 1;
    }

    // Partial range [1,4): 3 calls.
    g_calls = 0;
    vecCIP_Destroy(&arr[1], &arr[4]);
    if (g_calls != 3) {
        printf("FAIL partial calls=%d\n", g_calls);
        return 1;
    }

    printf("DESTROY_OK\n");
    return 0;
}
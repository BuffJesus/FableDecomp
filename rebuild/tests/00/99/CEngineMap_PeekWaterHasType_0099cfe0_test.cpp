#include <cstdio>

struct CEngineMap
{
    void* PeekWaterGetType(long a, long b);
    int   PeekWaterHasType(long a, long b);
};

int CEngineMap::PeekWaterHasType(long a, long b)
{
    return static_cast<bool>(PeekWaterGetType(a, b));
}

// Standalone definition of the helper for the behaviour test. It returns a
// (possibly null) pointer computed from the two coords so we can drive both
// the "has water" and "no water" branches.
void* CEngineMap::PeekWaterGetType(long a, long b)
{
    long v = a + b;               // 0 => "no type present"
    if (v == 0) return 0;
    return (void*)(v);            // non-null => "has type"
}

int main()
{
    CEngineMap m;
    int r1 = m.PeekWaterHasType(0, 0);     // sum 0  -> null  -> 0
    int r2 = m.PeekWaterHasType(1, 0);     // sum 1  -> ptr   -> 1
    int r3 = m.PeekWaterHasType(-3, 3);    // sum 0  -> null  -> 0
    int r4 = m.PeekWaterHasType(5, 7);     // sum 12 -> ptr   -> 1

    // result must be a normalized 0/1 boolean
    bool normalized = (r2 == 1) && (r4 == 1);
    if (r1 == 0 && r3 == 0 && normalized) {
        printf("PEEKWATER_OK\n");
        return 0;
    }
    printf("PEEKWATER_FAIL r1=%d r2=%d r3=%d r4=%d\n", r1, r2, r3, r4);
    return 1;
}
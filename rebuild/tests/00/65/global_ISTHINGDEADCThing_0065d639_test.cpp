#include <stdio.h>

struct CThing {
    char pad[0x91];
    unsigned char flags;
};

bool __fastcall IS_THING_DEAD(CThing** ppThing)
{
    CThing* t = *ppThing;
    if (t != 0 && (t->flags & 1) == 0)
        return false;
    return true;
}

int main()
{
    CThing alive;
    alive.flags = 0;
    CThing dead;
    dead.flags = 1;

    CThing* pAlive = &alive;
    CThing* pDead = &dead;
    CThing* pNull = 0;

    bool r1 = IS_THING_DEAD(&pAlive);   // false
    bool r2 = IS_THING_DEAD(&pDead);    // true
    bool r3 = IS_THING_DEAD(&pNull);    // true

    CThing two; two.flags = 2;          // bit0 clear -> alive
    CThing* pTwo = &two;
    bool r4 = IS_THING_DEAD(&pTwo);     // false

    CThing three; three.flags = 3;      // bit0 set -> dead
    CThing* pThree = &three;
    bool r5 = IS_THING_DEAD(&pThree);   // true

    if (!r1 && r2 && r3 && !r4 && r5) {
        printf("THING_DEAD_OK\n");
        return 0;
    }
    printf("FAIL %d %d %d %d %d\n", r1, r2, r3, r4, r5);
    return 1;
}
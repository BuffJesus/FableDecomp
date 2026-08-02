struct CThing {
    char pad[10];
    unsigned short mapNumber;

    void SetCurrentMapNumber_a(unsigned short newMapNumber)
    {
        mapNumber = newMapNumber;
    }
};

#include <cstdio>

int main()
{
    CThing t;
    for (int i = 0; i < 10; ++i) t.pad[i] = (char)0xCC;
    t.mapNumber = 0xBEEF;

    t.SetCurrentMapNumber_a(0x1234);

    bool padOk = true;
    for (int i = 0; i < 10; ++i) if (t.pad[i] != (char)0xCC) padOk = false;

    if (t.mapNumber == 0x1234 && padOk) {
        printf("PASS_MAPNUM_SET\n");
    } else {
        printf("FAIL mapNumber=%04x padOk=%d\n", (unsigned)t.mapNumber, (int)padOk);
    }

    return 0;
}
#include <cstdio>
#include <cstring>

class CThing {
public:
    char pad[0xc];
    short currentMapNumber;

    void SetCurrentMapNumber(short mapNumber);
};

void CThing::SetCurrentMapNumber(short mapNumber)
{
    currentMapNumber = mapNumber;
}

int main()
{
    CThing thing;
    memset(&thing, 0xCC, sizeof(thing));
    thing.SetCurrentMapNumber((short)0x1234);
    if (thing.currentMapNumber == (short)0x1234) {
        printf("PASS_SETMAPNUM_OK\n");
    } else {
        printf("FAIL_SETMAPNUM\n");
    }
    return 0;
}
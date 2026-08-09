
#include <stdio.h>

struct CBankFileAsyncData {
    char pad[0x4c];
    int state;
};

static void FreeBuffer_model(CBankFileAsyncData* self)
{
    int s = self->state;
    if (s != 3 && s != 1) {
        self->state = 1;
    }
}

int main()
{
    CBankFileAsyncData d;
    int fails = 0;

    d.state = 3; FreeBuffer_model(&d);
    if (d.state != 3) fails++;

    d.state = 1; FreeBuffer_model(&d);
    if (d.state != 1) fails++;

    d.state = 0; FreeBuffer_model(&d);
    if (d.state != 1) fails++;

    d.state = 2; FreeBuffer_model(&d);
    if (d.state != 1) fails++;

    d.state = 99; FreeBuffer_model(&d);
    if (d.state != 1) fails++;

    if (fails == 0) {
        printf("OK_00c25e00\n");
        return 0;
    }
    printf("FAIL %d\n", fails);
    return 1;
}
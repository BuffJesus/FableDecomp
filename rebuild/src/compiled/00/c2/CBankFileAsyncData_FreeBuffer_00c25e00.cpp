
struct CBankFileAsyncData {
    char pad[0x4c];
    int state;
};

void __fastcall FreeBuffer(CBankFileAsyncData* self)
{
    int s = self->state;
    if (s != 3 && s != 1) {
        self->state = 1;
    }
}
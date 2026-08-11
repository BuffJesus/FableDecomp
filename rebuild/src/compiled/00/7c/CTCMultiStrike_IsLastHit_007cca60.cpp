struct Inner {
    char pad[0x78];
    int* array;   // +0x78
};

struct CTCMultiStrike {
    char pad0[0x0c];
    Inner* inner;   // +0x0c
    int index;      // +0x10
    char pad1[0x08];
    int current;    // +0x1c
    bool IsLastHit();
};

bool CTCMultiStrike::IsLastHit()
{
    return this->current == (this->inner->array[this->index] - 1);
}
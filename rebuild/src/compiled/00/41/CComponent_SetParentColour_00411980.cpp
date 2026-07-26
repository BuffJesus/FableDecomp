struct CComponent {
    int* first;   // +0x0
    int* second;  // +0x4
};

static void copy_range(int* first, int* last, int* dst, CComponent* self)
{
    int n = (int)(last - first);
    while (n > 0) {
        *dst = *first;
        ++first;
        ++dst;
        --n;
    }
    self->second = dst;
}

void __fastcall CComponent_SetParentColour(CComponent* self)
{
    copy_range(self->second, self->second, self->first, self);
}
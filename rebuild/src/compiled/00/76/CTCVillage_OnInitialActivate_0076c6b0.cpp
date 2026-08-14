struct Inner;

void __fastcall Inner_Do(Inner* self, void* edx, unsigned char arg);

struct CTCVillage {
    Inner* p0;
    void*  p4;
    char   pad8[4];
    unsigned char c;
    unsigned char d;
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->d) {
        Inner_Do(self->p0, self->p4, self->c);
        self->d = 0;
    }
}
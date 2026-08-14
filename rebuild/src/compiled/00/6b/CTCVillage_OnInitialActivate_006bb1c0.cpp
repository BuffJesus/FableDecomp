struct Inner;

// callee at 0x6bb140: receives ecx=self, edx=arg2, stack=byte arg
void __fastcall Inner_Do(Inner* self, int edx, unsigned char v);

struct CTCVillage {
    Inner* p0;      // +0x00  -> ecx of callee
    int    v4;      // +0x04  -> edx of callee
    char   pad8;    // +0x08
    char   pad9;    // +0x09
    char   pada;    // +0x0a
    char   padb;    // +0x0b
    unsigned char bytec; // +0x0c
    char   flagd;   // +0x0d
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->flagd) {
        Inner_Do(self->p0, self->v4, self->bytec);
        self->flagd = 0;
    }
}
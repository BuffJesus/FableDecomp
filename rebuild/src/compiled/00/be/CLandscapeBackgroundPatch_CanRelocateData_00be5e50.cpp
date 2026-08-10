struct Inner { char pad[0x5c]; unsigned char flag; };
struct Mid { char pad[0xa4]; Inner* inner; };
struct CLandscapeBackgroundPatch { void* v; Mid* mid; };

bool __fastcall CanRelocateData(CLandscapeBackgroundPatch* self, int /*edx*/, long a) {
    (void)a;
    return self->mid->inner->flag == 0;
}
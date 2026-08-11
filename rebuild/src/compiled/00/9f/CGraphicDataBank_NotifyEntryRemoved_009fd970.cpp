struct CGraphicDataBank {
    char pad[0x1e0];
    char* f1e0;
};

void __fastcall NotifyEntryRemoved(CGraphicDataBank* self, int /*edx*/, unsigned long idx)
{
    char* base = self->f1e0;
    *(int*)(base + idx * 0x2c + 0x24) = -1;
}
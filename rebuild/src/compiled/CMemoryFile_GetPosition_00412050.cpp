struct CMemoryFile {
    int pad0;
    int pad4;
    long position;
};

long __fastcall CMemoryFile_GetPosition(const CMemoryFile* self) {
    return self->position;
}
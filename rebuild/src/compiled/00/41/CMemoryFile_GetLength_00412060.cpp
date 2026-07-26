struct CMemoryFile {
    int _pad0;
    int _pad4;
    int _pad8;
    long m_length;
};

long __fastcall CMemoryFile_GetLength(const CMemoryFile* self)
{
    return self->m_length;
}
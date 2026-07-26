struct CDiskFileWin32 {
    char pad[0x14];
    bool m_isOpen;
};

bool __fastcall CDiskFileWin32_IsOpen(const CDiskFileWin32* self)
{
    return self->m_isOpen;
}